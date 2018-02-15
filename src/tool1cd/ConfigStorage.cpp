#include <memory>

#include "ConfigStorage.h"
#include "Common.h"
#include "Base64.h"
#include "TempStream.h"

#include "UZLib.h"
#pragma comment (lib, "zlibstatic.lib")

const String spoint = ".";
const String sdynupdate = "_dynupdate_";
const int lsdynupdate = sdynupdate.Length();

extern Registrator msreg_g;

//********************************************************
// Класс ConfigStorageDirectory

//---------------------------------------------------------------------------
ConfigStorageDirectory::ConfigStorageDirectory(const String& _dir)
{
	fdir = _dir;
	if(*fdir.LastChar() != '\\') fdir += '\\'; // FIXME: разобраться в конструкторе ConfigStorageDirectory - это работа с файловой системой?
}

//---------------------------------------------------------------------------
ConfigFile* ConfigStorageDirectory::readfile(const String& path)
{
	ConfigFile* cf;
	String filename;

	filename = fdir + TStringBuilder(path).Replace('/', '\\')->ToString(); // FIXME: исправить чтение файла на boost::filesystem

	if(FileExists(filename))
	{
		cf = new ConfigFile;
		try
		{
			cf->str = new TFileStream(filename, fmOpenRead);
			cf->addin = nullptr;
			return cf;
		}
		catch(...)
		{
			// TODO: сообщить об ошибке и записать в log
			delete cf;
			return nullptr;
		}

	}
	else return nullptr;

}

//---------------------------------------------------------------------------
bool ConfigStorageDirectory::writefile(const String& path, TStream* str)
{
	String filename = fdir + TStringBuilder(path).Replace('/', '\\')->ToString(); // FIXME: исправить запись файла на boost::filesystem
	TFileStream f(filename, fmCreate);
	f.CopyFrom(str, 0);

	return true;
}

//---------------------------------------------------------------------------
String ConfigStorageDirectory::presentation()
{
	return fdir.SubString(1, fdir.Length() - 1);
}

//---------------------------------------------------------------------------
bool ConfigStorageDirectory::fileexists(const String& path)
{
	String filename;
	filename = fdir + TStringBuilder(path).Replace('/', '\\')->ToString(); // FIXME: исправить проверку существования файла на boost::filesystem
	return FileExists(filename);
}


//********************************************************
// Класс ConfigStorageCFFile

//---------------------------------------------------------------------------
ConfigStorageCFFile::ConfigStorageCFFile(const String& fname)
{
	filename = fname;
	cat = new V8Catalog(filename);
}

//---------------------------------------------------------------------------
ConfigStorageCFFile::~ConfigStorageCFFile()
{
	delete cat;
}

//---------------------------------------------------------------------------
ConfigFile* ConfigStorageCFFile::readfile(const String& path)
{
	V8Catalog* c;
	V8File* f;
	int i;
	ConfigFile* cf;

	if(!cat->isOpen()) return nullptr;

	String fname = TStringBuilder(path).Replace('/', '\\')->ToString();
	c = cat;
	for(i = fname.Pos("\\"); i; i = fname.Pos("\\"))
	{
		f = c->GetFile(fname.SubString(1, i - 1));
		if(!f) return nullptr;
		c = f->GetCatalog();
		if(!c) return nullptr;
		fname = fname.SubString(i + 1, fname.Length() - i);
	}
	f = c->GetFile(fname);
	if(!f) return nullptr;
	if(!f->Open()) return nullptr;
	cf = new ConfigFile;
	cf->str = f->get_stream();
	cf->str->Seek(0l, soBeginning);
	cf->addin = f;
	return cf;

}

//---------------------------------------------------------------------------
bool ConfigStorageCFFile::writefile(const String& path, TStream* str)
{
	V8Catalog* c;
	V8File* f;
	int i;

	if(!cat->isOpen()) return false;

	String fname = TStringBuilder(path).Replace('/', '\\')->ToString();
	c = cat;
	for(i = fname.Pos("\\"); i; i = fname.Pos("\\"))
	{
		c = c->CreateCatalog(fname.SubString(1, i - 1));
		fname = fname.SubString(i + 1, fname.Length() - i);
	}
	f = c->createFile(fname);
	f->Write(str);
	return true;
}

//---------------------------------------------------------------------------
String ConfigStorageCFFile::presentation()
{
	return filename;
}

//---------------------------------------------------------------------------
void ConfigStorageCFFile::close(ConfigFile* cf)
{
	V8File* f;

	f = (V8File*)cf->addin;
	f->Close();
	delete cf;
}

//---------------------------------------------------------------------------
bool ConfigStorageCFFile::fileexists(const String& path)
{
	// По сути, проверяется существование только каталога (файла верхнего уровня)
	// Это неправильно для формата 8.0 с файлом каталогом metadata. Но метод fileexists используется только для внешних файлов,
	// поэтому такой проверки достаточно

	V8File* f;
	int i;

	if(!cat->isOpen()) return false;

	String fname = TStringBuilder(path).Replace('/', '\\')->ToString();
	i = fname.Pos("\\");
	if(i) fname = fname.SubString(1, i - 1);
	f = cat->GetFile(fname);
	if(!f) return false;
	return true;
}


//********************************************************
// Класс ContainerFile

//---------------------------------------------------------------------------
ContainerFile::ContainerFile(TableFile* _f, const String& _name)
{


	file    = _f;
	name    = _name;
	stream  = nullptr;
	rstream = nullptr;
	cat     = nullptr;
	packed  = table_file_packed::unknown;
	dynno   = -3;
}

//---------------------------------------------------------------------------
ContainerFile::~ContainerFile()
{
	close();
}

//---------------------------------------------------------------------------
bool ContainerFile::open()
{
	TStream* ts;
	String tn;

	unsigned int i;
	Table* t;
	table_blob_file* addr;
	unsigned int maxpartno;

	if(stream)
	{
		stream->Seek(0l, soBeginning);
		return true;
	}
	t = file->t;
	addr = file->addr;
	maxpartno = file->maxpartno;

	if(maxpartno > 0) stream = new TTempStream;
	else stream = new TMemoryStream;

	if(packed == table_file_packed::unknown) packed = isPacked() ? table_file_packed::yes : table_file_packed::no;

	if(rstream)
	{
		if(packed == table_file_packed::yes) ts = rstream;
		else
		{
			stream = rstream;
			stream->Seek(0l, soBeginning);
			return true;
		}
	}
	else
	{
		if(packed == table_file_packed::yes)
		{
			if(maxpartno > 0) ts = new TTempStream;
			else ts = new TMemoryStream;
		}
		else ts = stream;

		for(i = 0; i <= maxpartno; ++i) t->readBlob(ts, addr[i].blob_start, addr[i].blob_length, false);
	}

	if(packed == table_file_packed::yes)
	{
		ts->Seek(0l, soBeginning);
		ZInflateStream(ts, stream);
		if(!rstream)
		{
			delete ts;
			DeleteFile(tn);
		}
	}

	stream->Seek(0l, soBeginning);
	return true;

}

//---------------------------------------------------------------------------
bool ContainerFile::ropen()
{

	unsigned int i;
	Table* t;
	table_blob_file* addr;
	unsigned int maxpartno;

	if(rstream)
	{
		rstream->Seek(0l, soBeginning);
		return true;
	}
	t = file->t;
	addr = file->addr;
	maxpartno = file->maxpartno;
	if(packed == table_file_packed::unknown) packed = isPacked() ? table_file_packed::yes : table_file_packed::no;
	if(packed == table_file_packed::no && stream)
	{
		rstream = stream;
		rstream->Seek(0l, soBeginning);
		return true;
	}

	if(maxpartno > 0) rstream = new TTempStream;
	else rstream = new TMemoryStream;

	for(i = 0; i <= maxpartno; ++i) t->readBlob(rstream, addr[i].blob_start, addr[i].blob_length, false);

	rstream->Seek(0l, soBeginning);
	return true;

}

//---------------------------------------------------------------------------
void ContainerFile::close()
{
	delete cat;
	cat = nullptr;
	if(stream != rstream)
	{
		delete stream;
		delete rstream;
	}
	else delete stream;
	stream = nullptr;
	rstream = nullptr;
}

//---------------------------------------------------------------------------
bool ContainerFile::isPacked()
{
	int i;
	String ext;

	if(name.CompareIC("DynamicallyUpdated") == 0) return false;
	if(name.CompareIC("SprScndInfo") == 0) return false;
	if(name.CompareIC("DBNamesVersion") == 0) return false;
	if(name.CompareIC("siVersions") == 0) return false;
	if(name.UpperCase().Pos("FO_VERSION") > 0) return false;
	if(name[1] == '{') return false;
	i = name.LastDelimiter(spoint);
	if(i)
	{
		ext = name.SubString(i + 1, name.Length() - i).UpperCase();
		if(ext.Compare("INF") == 0) return false;
		if(ext.Compare("PF") == 0) return false;
	}
	return true;

}


//********************************************************
// Структура дополнительных данных открытого файла класса ConfigStorageTable

//---------------------------------------------------------------------------
enum class ConfigStorageTableAddinVariant
{
	ContainerFile,
	V8File
};

//---------------------------------------------------------------------------
struct ConfigStorageTable_addin
{
	ConfigStorageTableAddinVariant variant;
	union
	{
		ContainerFile* tf;
		V8File* f;
	};
};

//********************************************************
// Класс ConfigStorageTable

//---------------------------------------------------------------------------
ConfigStorageTable::~ConfigStorageTable()
{
	std::map<String,ContainerFile*>::iterator pfiles;

	for(pfiles = files.begin(); pfiles != files.end(); ++pfiles)
	{
		delete pfiles->second;
	}

	delete base;
}

//---------------------------------------------------------------------------
ConfigFile* ConfigStorageTable::readfile(const String& path)
{
	ContainerFile* tf;
	std::map<String,ContainerFile*>::iterator pfiles;
	V8Catalog* c;
	V8File* f;
	int i;
	String fname;
	String r_name;
	ConfigFile* cf;
	ConfigStorageTable_addin* cfa;

	if(!ready) return nullptr;

	fname = TStringBuilder(path).Replace('/', '\\')->ToString();
	i = fname.Pos("\\");
	if(i)
	{
		r_name = fname.SubString(1, i - 1);
		fname = fname.SubString(i + 1, fname.Length() - i);
	}
	else
	{
		r_name = fname;
		fname = "";
	}

	pfiles = files.find(r_name.UpperCase());
	if(pfiles == files.end()) return nullptr;
	tf = pfiles->second;

	tf->open();
	if(!fname.IsEmpty())
	{
		if(!tf->cat) tf->cat = new V8Catalog(tf->stream, false, true);
		c = tf->cat;
		for(i = fname.Pos("\\"); i; i = fname.Pos("\\"))
		{
			f = c->GetFile(fname.SubString(1, i - 1));
			if(!f) return nullptr;
			c = f->GetCatalog();
			if(!c) return nullptr;
			fname = fname.SubString(i + 1, fname.Length() - i);
		}
		f = c->GetFile(fname);
		if(!f) return nullptr;
		if(!f->Open()) return nullptr;
		cf = new ConfigFile;
		cfa = new ConfigStorageTable_addin;
		cfa->variant = ConfigStorageTableAddinVariant::V8File;
		cfa->f = f;
		cf->str = f->get_stream();
		cf->str->Seek(0l, soBeginning);
		cf->addin = cfa;
	}
	else
	{
		cf = new ConfigFile;
		cfa = new ConfigStorageTable_addin;
		cfa->variant = ConfigStorageTableAddinVariant::ContainerFile;
		cfa->tf = tf;
		cf->str = tf->stream;
		cf->str->Seek(0l, soBeginning);
		cf->addin = cfa;
	}

	return cf;

}

//---------------------------------------------------------------------------
bool ConfigStorageTable::writefile(const String& path, TStream* str)
{
	return false; // TODO Запись в таблицы пока не поддерживается
}

//---------------------------------------------------------------------------
void ConfigStorageTable::close(ConfigFile* cf)
{
	ConfigStorageTable_addin* cfa;

	cfa = (ConfigStorageTable_addin*)cf->addin;
	if(cfa->variant == ConfigStorageTableAddinVariant::ContainerFile)
	{
		cfa->tf->close();
	}
	else if(cfa->variant == ConfigStorageTableAddinVariant::V8File)
	{
		cfa->f->Close();
	}
	delete cfa;
	delete cf;
}

//---------------------------------------------------------------------------
bool ConfigStorageTable::save_config(const boost::filesystem::path &file_name)
{
	if(!ready) {
		return false;
	}

	if(boost::filesystem::exists(file_name)) {
		boost::filesystem::remove_all(file_name);
	}

	size_t prevj = 101;
	size_t i = 1;

	std::unique_ptr<V8Catalog> catalog (new V8Catalog(file_name.string(), false));
	for(auto pfiles: files)
	{
		++i;
		size_t j = i * 100 / files.size();
		if(j != prevj)
		{
			prevj = j;
			msreg_g.Status(String(j) + "%");
		}

		ContainerFile* tf = pfiles.second;
		if(tf->ropen())
		{
			V8File* f = catalog->createFile(tf->name);
			f->time_create(tf->file->ft_create);
			f->time_modify(tf->file->ft_modify);
			f->WriteAndClose(tf->rstream);

			tf->close();
		}
	}

	msreg_g.Status("");
	return true;
}

//---------------------------------------------------------------------------
bool ConfigStorageTable::fileexists(const String& path)
{
	// По сути, проверяется существование только каталога (файла записи верхнего уровня)
	// Это неправильно для формата 8.0 с файлом каталогом metadata. Но метод fileexists используется только для внешних файлов, поэтому такой проверки достаточно

	std::map<String,ContainerFile*>::iterator pfiles;
	int i;
	String fname;

	if(!ready) return false;

	fname = TStringBuilder(path).Replace('/', '\\')->ToString();
	i = fname.Pos("\\");
	if(i) fname = fname.SubString(1, i - 1);

	pfiles = files.find(fname.UpperCase());
	if(pfiles == files.end()) return false;
	return true;
}

//********************************************************
// Класс ConfigStorageTableConfig

//---------------------------------------------------------------------------
ConfigStorageTableConfig::ConfigStorageTableConfig(TableFiles* tabf, T_1CD* _base) : ConfigStorageTable(_base)
{
	int m;
	String s;
	String name, ext;
	TableFile* tf;
	std::map<String,TableFile*>::iterator pfilesmap;
	std::map<String,ContainerFile*>::iterator pfiles;
	TableFile* _DynamicallyUpdated;
	ContainerFile* DynamicallyUpdated;
	tree* tt;
	tree* ct;
	std::vector<BinaryGuid> dynup;
	BinaryGuid g;
	Table* tab;
	int dynno;
	ContainerFile* pcf;

	ready = tabf->getready();
	if(!ready) return;

	present = tabf->gettable()->getbase()->getfilename() + "\\CONFIG";

	tab = tabf->gettable();
	_DynamicallyUpdated = tabf->getfile("DynamicallyUpdated");

	if(_DynamicallyUpdated)
	{
		DynamicallyUpdated = new ContainerFile(_DynamicallyUpdated, _DynamicallyUpdated->name);
		DynamicallyUpdated->open();
		s = tab->getbase()->getfilename() + "\\" + tab->getname() + "\\" + DynamicallyUpdated->name;
		tt = parse_1Cstream(DynamicallyUpdated->stream, s);
		if(!tt)
		{
			msreg_g.AddError("Ошибка разбора файла DynamicallyUpdated"
				, "Путь", s);
		}
		else
		{
			ct = tt->get_first();
			if(!ct)
			{
				msreg_g.AddError("Ошибка разбора файла DynamicallyUpdated"
					, "Путь", s);
			}
			else
			{
				ct = ct->get_first();
				if(!ct)
				{
					msreg_g.AddError("Ошибка разбора файла DynamicallyUpdated"
						, "Путь", s);
				}
				else
				{
					ct = ct->get_next();
					if(!ct)
					{
						msreg_g.AddError("Ошибка разбора файла DynamicallyUpdated"
							, "Путь", s);
					}
					else
					{
						if(ct->get_type() != node_type::nd_number)
						{
							msreg_g.AddError("Ошибка разбора файла DynamicallyUpdated"
								, "Путь", s);
						}
						else
						{
							int ndynup = ct->get_value().ToIntDef(0);
							if(ndynup > 0)
							{
								dynup.reserve(ndynup);
								for(m = 0; m < ndynup; ++m)
								{
									ct = ct->get_next();
									dynup.emplace_back(BinaryGuid(ct->get_value()));
								}
							}
							else ndynup = 0;
						}

					}
				}
			}
		}
		delete DynamicallyUpdated;
	}

	for (pfilesmap = tabf->files().begin(); pfilesmap != tabf->files().end(); ++pfilesmap)
	{
		tf = pfilesmap->second;
		if(tf == _DynamicallyUpdated) continue;
		if(tf->addr->blob_length == 0) continue;
		s = tf->name;
		m = s.LastDelimiter(spoint);
		if(m)
		{
			name = s.SubString(1, m - 1);
			ext = s.SubString(m + 1, s.Length() - m);
		}
		else
		{
			name = s;
			ext = "";
		}

		if(ext.CompareIC("new") == 0)
		{
			ext = "";
			dynno = - 2;
		}
		else dynno = -1;

		m = name.Pos(sdynupdate);
		if(m)
		{
			s = name.SubString(m + lsdynupdate, name.Length() - m - lsdynupdate + 1);
			name = name.SubString(1, m - 1);
			g = BinaryGuid(s);
			if(!dynup.empty())
			{
				auto found = std::find(dynup.begin(), dynup.end(), g);
				if (found == dynup.end()) {
					dynno = -2;
				} else {
					dynno = found - dynup.begin();
				}
			}
			else dynno = -2;
		}

		if(!ext.IsEmpty()) name = name + spoint + ext;
		s = name.UpperCase();

		pfiles = files.find(s);
		if(pfiles == files.end())
		{
			pcf = new ContainerFile(tf, name);
			files[s] = pcf;
			pcf->dynno = dynno;
		}
		else
		{
			pcf = pfiles->second;
			if(pcf->dynno < dynno)
			{
				pcf->file = tf;
				pcf->dynno = dynno;
			}
		}
	}

}

//---------------------------------------------------------------------------
String ConfigStorageTableConfig::presentation()
{
	return present;
}

//********************************************************
// Класс ConfigStorageTableConfigSave

//---------------------------------------------------------------------------
ConfigStorageTableConfigSave::ConfigStorageTableConfigSave(TableFiles* tabc, TableFiles* tabcs, T_1CD* _base) : ConfigStorageTable(_base)
{
	int m;
	String s;
	String name, ext;
	TableFile* tf;
	std::map<String,TableFile*>::iterator pfilesmap;
	std::map<String,ContainerFile*>::iterator pfiles;
	TableFile* _DynamicallyUpdated;
	ContainerFile* DynamicallyUpdated;
	TableFile* _deleted;
	ContainerFile* deleted;
	tree* tt;
	tree* ct;
	BinaryGuid* dynup;
	int ndynup = 0;
	BinaryGuid g;
	Table* tab;
	int dynno;
	ContainerFile* pcf;
	std::set<String> del;
	std::set<String>::iterator pdel;

	ready = tabc->getready();
	if(!ready) return;
	ready = tabcs->getready();
	if(!ready) return;

	present = tabc->gettable()->getbase()->getfilename() + "\\CONFIGSAVE";

	tab = tabcs->gettable();
	_deleted = tabcs->getfile("deleted");
	if(_deleted)
	{
		deleted = new ContainerFile(_deleted, _deleted->name);
		deleted->open();
		s = tab->getbase()->getfilename() + "\\" + tab->getname() + "\\" + deleted->name;
		tt = parse_1Cstream(deleted->stream, s);
		if(!tt)
		{
			msreg_g.AddError("Ошибка разбора файла deleted"
				, "Путь", s);
		}
		else
		{
			ct = tt->get_first();
			if(!ct)
			{
				msreg_g.AddError("Ошибка разбора файла DynamicallyUpdated"
					, "Путь", s);
			}
			{
				for(m = ct->get_value().ToIntDef(0); m; --m)
				{
					ct = ct->get_next();
					if(!ct)
					{
						msreg_g.AddError("Ошибка разбора файла DynamicallyUpdated"
							, "Путь", s);
						break;
					}
					del.insert(ct->get_value().UpperCase());
					ct = ct->get_next();
					if(!ct)
					{
						msreg_g.AddError("Ошибка разбора файла DynamicallyUpdated"
							, "Путь", s);
						break;
					}
				}
			}

		}
		delete deleted;
	}

	tab = tabc->gettable();
	_DynamicallyUpdated = tabc->getfile("DynamicallyUpdated");

	dynup = nullptr;
	if(_DynamicallyUpdated)
	{
		DynamicallyUpdated = new ContainerFile(_DynamicallyUpdated, _DynamicallyUpdated->name);
		DynamicallyUpdated->open();
		s = tab->getbase()->getfilename() + "\\" + tab->getname() + "\\" + DynamicallyUpdated->name;
		tt = parse_1Cstream(DynamicallyUpdated->stream, s);
		if(!tt)
		{
			msreg_g.AddError("Ошибка разбора файла DynamicallyUpdated"
				, "Путь", s);
		}
		else
		{
			ct = tt->get_first();
			if(!ct)
			{
				msreg_g.AddError("Ошибка разбора файла DynamicallyUpdated"
					, "Путь", s);
			}
			else
			{
				ct = ct->get_first();
				if(!ct)
				{
					msreg_g.AddError("Ошибка разбора файла DynamicallyUpdated"
						, "Путь", s);
				}
				else
				{
					ct = ct->get_next();
					if(!ct)
					{
						msreg_g.AddError("Ошибка разбора файла DynamicallyUpdated"
							, "Путь", s);
					}
					else
					{
						if(ct->get_type() != node_type::nd_number)
						{
							msreg_g.AddError("Ошибка разбора файла DynamicallyUpdated"
								, "Путь", s);
						}
						else
						{
							ndynup = ct->get_value().ToIntDef(0);
							if(ndynup > 0)
							{
								dynup = new BinaryGuid[ndynup];
								for(m = 0; m < ndynup; ++m)
								{
									ct = ct->get_next();
									string_to_GUID(ct->get_value(), &dynup[m]);
								}
							}
							else ndynup = 0;
						}

					}
				}
			}
		}
		delete DynamicallyUpdated;
	}

	m = ndynup + 2;

	for(pfilesmap = tabcs->files().begin(); pfilesmap != tabcs->files().end(); ++pfilesmap)
	{
		tf = pfilesmap->second;
		if(tf == _deleted) continue;
		if(tf->addr->blob_length == 0) continue;
		name = tf->name;
		s = name.UpperCase();

		pfiles = files.find(s);
		if(pfiles == files.end())
		{
			pcf = new ContainerFile(tf, name);
			files[s] = pcf;
			pcf->dynno = m;
		}
		else
		{
			msreg_g.AddError("Ошибка чтения CONFIGSAVE. Повторяющееся имя файла"
				, "Имя файла", s);
		}
	}

	for(pfilesmap = tabc->files().begin(); pfilesmap != tabc->files().end(); ++pfilesmap)
	{
		tf = pfilesmap->second;
		if(tf == _DynamicallyUpdated) continue;
		if(tf->addr->blob_length == 0) continue;
		s = tf->name;

		pdel = del.find(s.UpperCase());
		if(pdel != del.end()) continue;

		m = s.LastDelimiter(spoint);
		if(m)
		{
			name = s.SubString(1, m - 1);
			ext = s.SubString(m + 1, s.Length() - m);
		}
		else
		{
			name = s;
			ext = "";
		}
		if(ext.CompareIC("new") == 0)
		{
			ext = "";
			dynno = ndynup + 1;
		}
		else dynno = -1;

		m = name.Pos(sdynupdate);
		if(m)
		{
			s = name.SubString(m + lsdynupdate, name.Length() - m - lsdynupdate + 1);
			name = name.SubString(1, m - 1);
			string_to_GUID(s, &g);
			if(dynup)
			{
				for(m = 0; m < ndynup; ++m) if(g == dynup[m]) break;
				if(m >= ndynup) dynno = -2;
				else dynno = m;
			}
			else dynno = -2;
		}

		if(!ext.IsEmpty()) name = name + spoint + ext;
		s = name.UpperCase();

		pfiles = files.find(s);
		if(pfiles == files.end())
		{
			pcf = new ContainerFile(tf, name);
			files[s] = pcf;
			pcf->dynno = dynno;
		}
		else
		{
			pcf = pfiles->second;
			if(pcf->dynno < dynno)
			{
				pcf->file = tf;
				pcf->dynno = dynno;
			}
		}
	}

	if(dynup) delete[] dynup;

}

//---------------------------------------------------------------------------
String ConfigStorageTableConfigSave::presentation()
{
	return present;
}

//********************************************************
// Класс ConfigStorageTableConfigCas

//---------------------------------------------------------------------------
ConfigStorageTableConfigCas::ConfigStorageTableConfigCas(TableFiles* tabc, const String& configver, T_1CD* _base) : ConfigStorageTable(_base)
{
	int m;
	String s, name, hashname;
	TableFile* _configinfo;
	ContainerFile* configinfo;
	tree* tt;
	tree* ct;
	TableFile* tf;
	ContainerFile* pcf;
	TMemoryStream* stream;

	ready = tabc->getready();
	if(!ready) return;

	present = tabc->gettable()->getbase()->getfilename() + "\\CONFIGCAS";
	s = present + "\\" + configver;

	_configinfo = tabc->getfile(configver);
	if(!_configinfo)
	{
		msreg_g.AddError("Ошибка поиска файла"
			, "Путь", s
			, "Имя", configver
			, "Имя файла", "configinfo");
		return;
	}

	configinfo = new ContainerFile(_configinfo, "configinfo");
	files["CONFIGINFO"] = configinfo;
	configinfo->open();
	tt = parse_1Cstream(configinfo->stream, s);
	if(!tt)
	{
		msreg_g.AddError("Ошибка разбора файла configinfo"
			, "Путь", s);
		delete tt;
		return;
	}
	ct = tt->get_first();
	if(!ct)
	{
		msreg_g.AddError("Ошибка разбора файла configinfo"
			, "Путь", s);
		delete tt;
		return;
	}
	ct = ct->get_next();
	if(!ct)
	{
		msreg_g.AddError("Ошибка разбора файла configinfo"
			, "Путь", s);
		delete tt;
		return;
	}
	ct = ct->get_next();
	if(!ct)
	{
		msreg_g.AddError("Ошибка разбора файла configinfo"
			, "Путь", s);
		delete tt;
		return;
	}
	ct = ct->get_first();
	if(!ct)
	{
		msreg_g.AddError("Ошибка разбора файла configinfo"
			, "Путь", s);
		delete tt;
		return;
	}

	stream = new TMemoryStream;
	for(m = ct->get_value().ToIntDef(0); m; --m)
	{
		ct = ct->get_next();
		if(!ct)
		{
			msreg_g.AddError("Ошибка разбора файла configinfo"
				, "Путь", s);
			delete tt;
			delete stream;
			return;
		}
		if(ct->get_type() != node_type::nd_string)
		{
			msreg_g.AddError("Ошибка разбора файла configinfo"
				, "Путь", s);
			delete tt;
			delete stream;
			return;
		}
		name = ct->get_value();

		ct = ct->get_next();
		if(!ct)
		{
			msreg_g.AddError("Ошибка разбора файла configinfo"
				, "Путь", s);
			delete tt;
			delete stream;
			return;
		}
		if(ct->get_type() != node_type::nd_binary2)
		{
			msreg_g.AddError("Ошибка разбора файла configinfo"
				, "Путь", s);
			delete tt;
			delete stream;
			return;
		}
		stream->Seek(0l, soBeginning);
		base64_decode(ct->get_value(), stream);
		stream->Seek(0l, soBeginning);
		hashname = hexstring(stream);

		tf = tabc->getfile(hashname);
		if(tf)
		{
			pcf = new ContainerFile(tf, name);
			files[name.UpperCase()] = pcf;
		}
		else
		{
			msreg_g.AddError("Ошибка поиска файла"
				, "Путь", s
				, "Имя", hashname
				, "Имя файла", name);
			delete tt;
			delete stream;
			return;
		}
	}

	delete tt;
	delete stream;

}

//---------------------------------------------------------------------------
String ConfigStorageTableConfigCas::presentation()
{
	return present;
}

//********************************************************
// Класс ConfigStorageTableConfigCasSave

//---------------------------------------------------------------------------
ConfigStorageTableConfigCasSave::ConfigStorageTableConfigCasSave(TableFiles* tabc, TableFiles* tabcs, const BinaryGuid& uid, const String& configver, T_1CD* _base) : ConfigStorageTable(_base)
{
	int m;
	String s, name, hashname;
	TableFile* _configinfo;
	ContainerFile* configinfo;
	tree* tt;
	tree* ct;
	TableFile* tf;
	ContainerFile* pcf;
	TMemoryStream* stream;
	String g;
	int gl;
	std::map<String,TableFile*>::iterator ptf;


	ready = tabc->getready();
	if(!ready) return;
	ready = tabcs->getready();
	if(!ready) return;

	configinfo = nullptr;
	present = tabcs->gettable()->getbase()->getfilename() + "\\CONFIGCASSAVE";

	g = uid.as_1C() + "__";
	gl = g.Length();
	for(ptf = tabcs->files().begin(); ptf != tabcs->files().end(); ++ptf)
	{
		if(ptf->first.SubString(1, gl).CompareIC(g) == 0)
		{
			tf = ptf->second;
			pcf = new ContainerFile(tf, tf->name.SubString(gl + 1, tf->name.Length() - gl));
			if(pcf->name.CompareIC("configinfo") == 0) configinfo = pcf;
			files[pcf->name.UpperCase()] = pcf;
		}
	}

	if(!configinfo)
	{
		if(files.size())
		{
			msreg_g.AddError("Ошибка поиска файла"
				, "Путь", present
				, "Имя", g + "configinfo"
				, "Имя файла", "configinfo");
			return;
		}

		s = tabc->gettable()->getbase()->getfilename() + "\\CONFIGCAS\\" + configver;
		_configinfo = tabc->getfile(configver);
		if(!_configinfo)
		{
			msreg_g.AddError("Ошибка поиска файла"
				, "Путь", s
				, "Имя", configver
				, "Имя файла", "configinfo");
			return;
		}

		configinfo = new ContainerFile(_configinfo, "configinfo");
		files["CONFIGINFO"] = configinfo;
	}
	else s = present + "\\" + gl + "configinfo";

	configinfo->open();
	tt = parse_1Cstream(configinfo->stream, s);
	if(!tt)
	{
		msreg_g.AddError("Ошибка разбора файла configinfo"
			, "Путь", s);
		delete tt;
		return;
	}
	ct = tt->get_first();
	if(!ct)
	{
		msreg_g.AddError("Ошибка разбора файла configinfo"
			, "Путь", s);
		delete tt;
		return;
	}
	ct = ct->get_next();
	if(!ct)
	{
		msreg_g.AddError("Ошибка разбора файла configinfo"
			, "Путь", s);
		delete tt;
		return;
	}
	ct = ct->get_next();
	if(!ct)
	{
		msreg_g.AddError("Ошибка разбора файла configinfo"
			, "Путь", s);
		delete tt;
		return;
	}
	ct = ct->get_first();
	if(!ct)
	{
		msreg_g.AddError("Ошибка разбора файла configinfo"
			, "Путь", s);
		delete tt;
		return;
	}

	stream = new TMemoryStream;
	for(m = ct->get_value().ToIntDef(0); m; --m)
	{
		ct = ct->get_next();
		if(!ct)
		{
			msreg_g.AddError("Ошибка разбора файла configinfo"
				, "Путь", s);
			delete tt;
			delete stream;
			return;
		}
		if(ct->get_type() != node_type::nd_string)
		{
			msreg_g.AddError("Ошибка разбора файла configinfo"
				, "Путь", s);
			delete tt;
			delete stream;
			return;
		}
		name = ct->get_value();

		ct = ct->get_next();
		if(files.find(name.UpperCase()) != files.end()) continue;

		if(!ct)
		{
			msreg_g.AddError("Ошибка разбора файла configinfo"
				, "Путь", s);
			delete tt;
			delete stream;
			return;
		}
		if(ct->get_type() != node_type::nd_binary2)
		{
			msreg_g.AddError("Ошибка разбора файла configinfo"
				, "Путь", s);
			delete tt;
			delete stream;
			return;
		}
		stream->Seek(0l, soBeginning);
		base64_decode(ct->get_value(), stream);
		stream->Seek(0l, soBeginning);
		hashname = hexstring(stream);

		tf = tabc->getfile(hashname);
		if(tf)
		{
			pcf = new ContainerFile(tf, name);
			files[name.UpperCase()] = pcf;
		}
		else
		{
			msreg_g.AddError("Ошибка поиска файла"
				, "Путь", s
				, "Имя", hashname
				, "Имя файла", name);
			delete tt;
			delete stream;
			return;
		}
	}

	delete tt;
	delete stream;

}

//---------------------------------------------------------------------------
String ConfigStorageTableConfigCasSave::presentation()
{
	return present;
}
