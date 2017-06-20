
#ifndef MessageRegistrationH
#define MessageRegistrationH

#include <System.Classes.hpp>
#pragma hdrstop

//---------------------------------------------------------------------------
enum MessageState
{
	msEmpty = -1,
	msSuccesfull = 0,
	msWarning = 1,
	msInfo = 2,
	msError = 3,
	msWait = 4,
	msHint = 5
};

class IMessageRegistration
{
public:
	virtual ~IMessageRegistration() {}
	virtual void AddMessage(const String& description, const MessageState mstate, TStringList* param = NULL) = 0;
	virtual void Status(const String& message) = 0;
	virtual void AddError(const String& description) = 0;
	virtual void AddError(const String& description,
				const String& parname1, const String& par1
	) = 0;
	virtual void AddError(const String& description,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2
	) = 0;
	virtual void AddError(const String& description,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2,
				const String& parname3, const String& par3
	) = 0;
	virtual void AddError(const String& description,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2,
				const String& parname3, const String& par3,
				const String& parname4, const String& par4
	) = 0;
	virtual void AddError(const String& description,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2,
				const String& parname3, const String& par3,
				const String& parname4, const String& par4,
				const String& parname5, const String& par5
	) = 0;
	virtual void AddError(const String& description,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2,
				const String& parname3, const String& par3,
				const String& parname4, const String& par4,
				const String& parname5, const String& par5,
				const String& parname6, const String& par6
	) = 0;
	virtual void AddError(const String& description,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2,
				const String& parname3, const String& par3,
				const String& parname4, const String& par4,
				const String& parname5, const String& par5,
				const String& parname6, const String& par6,
				const String& parname7, const String& par7
	) = 0;

	virtual void AddMessage_(const String& description, const MessageState mstate,
				const String& parname1, const String& par1
	) = 0;
	virtual void AddMessage_(const String& description, const MessageState mstate,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2
	) = 0;
	virtual void AddMessage_(const String& description, const MessageState mstate,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2,
				const String& parname3, const String& par3
	) = 0;
	virtual void AddMessage_(const String& description, const MessageState mstate,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2,
				const String& parname3, const String& par3,
				const String& parname4, const String& par4
	) = 0;
	virtual void AddMessage_(const String& description, const MessageState mstate,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2,
				const String& parname3, const String& par3,
				const String& parname4, const String& par4,
				const String& parname5, const String& par5
	) = 0;
	virtual void AddMessage_(const String& description, const MessageState mstate,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2,
				const String& parname3, const String& par3,
				const String& parname4, const String& par4,
				const String& parname5, const String& par5,
				const String& parname6, const String& par6
	) = 0;
	virtual void AddMessage_(const String& description, const MessageState mstate,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2,
				const String& parname3, const String& par3,
				const String& parname4, const String& par4,
				const String& parname5, const String& par5,
				const String& parname6, const String& par6,
				const String& parname7, const String& par7
	) = 0;

	virtual void AddDebugMessage(const String& description, const MessageState mstate) = 0;

	virtual void AddDebugMessage(const String& description, const MessageState mstate,
				const String& parname1, const String& par1
	) = 0;
	virtual void AddDebugMessage(const String& description, const MessageState mstate,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2
	) = 0;
	virtual void AddDebugMessage(const String& description, const MessageState mstate,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2,
				const String& parname3, const String& par3
	) = 0;
	virtual void AddDebugMessage(const String& description, const MessageState mstate,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2,
				const String& parname3, const String& par3,
				const String& parname4, const String& par4
	) = 0;

	virtual void AddDebugMessage(const String& description, const MessageState mstate,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2,
				const String& parname3, const String& par3,
				const String& parname4, const String& par4,
				const String& parname5, const String& par5
	) = 0;
	virtual void AddDebugMessage(const String& description, const MessageState mstate,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2,
				const String& parname3, const String& par3,
				const String& parname4, const String& par4,
				const String& parname5, const String& par5,
				const String& parname6, const String& par6
	) = 0;
	virtual void AddDebugMessage(const String& description, const MessageState mstate,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2,
				const String& parname3, const String& par3,
				const String& parname4, const String& par4,
				const String& parname5, const String& par5,
				const String& parname6, const String& par6,
				const String& parname7, const String& par7
	) = 0;
};

//---------------------------------------------------------------------------
class MessageRegistrator: public IMessageRegistration
{
private:
	bool DebugMessage;

public:
	MessageRegistrator();
	virtual ~MessageRegistrator();
	void setDebugMode(bool dstate);
	bool getDebugMode();
	virtual void AddMessage(const String& description, const MessageState mstate, TStringList* param = NULL) = 0;
	virtual void Status(const String& message) = 0;
	void AddError(const String& description);
	void AddError(const String& description,
				const String& parname1, const String& par1
	);
	void AddError(const String& description,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2
	);
	void AddError(const String& description,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2,
				const String& parname3, const String& par3
	);
	void AddError(const String& description,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2,
				const String& parname3, const String& par3,
				const String& parname4, const String& par4
	);
	void AddError(const String& description,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2,
				const String& parname3, const String& par3,
				const String& parname4, const String& par4,
				const String& parname5, const String& par5
	);
	void AddError(const String& description,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2,
				const String& parname3, const String& par3,
				const String& parname4, const String& par4,
				const String& parname5, const String& par5,
				const String& parname6, const String& par6
	);
	void AddError(const String& description,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2,
				const String& parname3, const String& par3,
				const String& parname4, const String& par4,
				const String& parname5, const String& par5,
				const String& parname6, const String& par6,
				const String& parname7, const String& par7
	);

	void AddMessage_(const String& description, const MessageState mstate,
				const String& parname1, const String& par1
	);
	void AddMessage_(const String& description, const MessageState mstate,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2
	);
	void AddMessage_(const String& description, const MessageState mstate,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2,
				const String& parname3, const String& par3
	);
	void AddMessage_(const String& description, const MessageState mstate,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2,
				const String& parname3, const String& par3,
				const String& parname4, const String& par4
	);
	void AddMessage_(const String& description, const MessageState mstate,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2,
				const String& parname3, const String& par3,
				const String& parname4, const String& par4,
				const String& parname5, const String& par5
	);
	void AddMessage_(const String& description, const MessageState mstate,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2,
				const String& parname3, const String& par3,
				const String& parname4, const String& par4,
				const String& parname5, const String& par5,
				const String& parname6, const String& par6
	);
	void AddMessage_(const String& description, const MessageState mstate,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2,
				const String& parname3, const String& par3,
				const String& parname4, const String& par4,
				const String& parname5, const String& par5,
				const String& parname6, const String& par6,
				const String& parname7, const String& par7
	);

	void AddDebugMessage(const String& description, const MessageState mstate);

	void AddDebugMessage(const String& description, const MessageState mstate,
				const String& parname1, const String& par1
	);
	void AddDebugMessage(const String& description, const MessageState mstate,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2
	);
	void AddDebugMessage(const String& description, const MessageState mstate,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2,
				const String& parname3, const String& par3
	);
	void AddDebugMessage(const String& description, const MessageState mstate,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2,
				const String& parname3, const String& par3,
				const String& parname4, const String& par4
	);

	void AddDebugMessage(const String& description, const MessageState mstate,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2,
				const String& parname3, const String& par3,
				const String& parname4, const String& par4,
				const String& parname5, const String& par5
	);
	void AddDebugMessage(const String& description, const MessageState mstate,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2,
				const String& parname3, const String& par3,
				const String& parname4, const String& par4,
				const String& parname5, const String& par5,
				const String& parname6, const String& par6
	);
	void AddDebugMessage(const String& description, const MessageState mstate,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2,
				const String& parname3, const String& par3,
				const String& parname4, const String& par4,
				const String& parname5, const String& par5,
				const String& parname6, const String& par6,
				const String& parname7, const String& par7
	);
};

//---------------------------------------------------------------------------

class IControlMessageRegistration
{
public:
	virtual ~IControlMessageRegistration() {}
	virtual void AddMessageRegistrator(MessageRegistrator* messageregistrator) = 0;
	virtual void RemoveMessageRegistrator() = 0;
};

class Registrator: public IMessageRegistration, public IControlMessageRegistration
{
public:
	inline void AddMessage(const String& description, const MessageState mstate,
					TStringList* param = NULL);
	inline void Status(const String& message);
	inline void AddError(const String& description);
	inline void AddError(const String& description,
					const String& parname1, const String& par1);
	inline void AddError(const String& description,
					const String& parname1, const String& par1,
					const String& parname2, const String& par2
		);
	inline void AddError(const String& description,
					const String& parname1, const String& par1,
					const String& parname2, const String& par2,
					const String& parname3, const String& par3
		);
	inline void AddError(const String& description,
					const String& parname1, const String& par1,
					const String& parname2, const String& par2,
					const String& parname3, const String& par3,
					const String& parname4, const String& par4
		);
	inline void AddError(const String& description,
					const String& parname1, const String& par1,
					const String& parname2, const String& par2,
					const String& parname3, const String& par3,
					const String& parname4, const String& par4,
					const String& parname5, const String& par5
		);
	inline void AddError(const String& description,
					const String& parname1, const String& par1,
					const String& parname2, const String& par2,
					const String& parname3, const String& par3,
					const String& parname4, const String& par4,
					const String& parname5, const String& par5,
					const String& parname6, const String& par6
		);
	inline void AddError(const String& description,
					const String& parname1, const String& par1,
					const String& parname2, const String& par2,
					const String& parname3, const String& par3,
					const String& parname4, const String& par4,
					const String& parname5, const String& par5,
					const String& parname6, const String& par6,
					const String& parname7, const String& par7
		);
	inline void AddMessage_(const String& description, const MessageState mstate,
					const String& parname1, const String& par1);
	inline void AddMessage_(const String& description, const MessageState mstate,
					const String& parname1, const String& par1,
					const String& parname2, const String& par2
		);
	inline void AddMessage_(const String& description, const MessageState mstate,
					const String& parname1, const String& par1,
					const String& parname2, const String& par2,
					const String& parname3, const String& par3
		);
	inline void AddMessage_(const String& description, const MessageState mstate,
					const String& parname1, const String& par1,
					const String& parname2, const String& par2,
					const String& parname3, const String& par3,
					const String& parname4, const String& par4
		);
	inline void AddMessage_(const String& description, const MessageState mstate,
					const String& parname1, const String& par1,
					const String& parname2, const String& par2,
					const String& parname3, const String& par3,
					const String& parname4, const String& par4,
					const String& parname5, const String& par5
		);
	inline void AddMessage_(const String& description, const MessageState mstate,
					const String& parname1, const String& par1,
					const String& parname2, const String& par2,
					const String& parname3, const String& par3,
					const String& parname4, const String& par4,
					const String& parname5, const String& par5,
					const String& parname6, const String& par6
		);
	inline void AddMessage_(const String& description, const MessageState mstate,
					const String& parname1, const String& par1,
					const String& parname2, const String& par2,
					const String& parname3, const String& par3,
					const String& parname4, const String& par4,
					const String& parname5, const String& par5,
					const String& parname6, const String& par6,
					const String& parname7, const String& par7
		);
	inline void AddDebugMessage(const String& description, const MessageState mstate);
	inline void AddDebugMessage(const String& description, const MessageState mstate,
					const String& parname1, const String& par1
		);
	inline void AddDebugMessage(const String& description, const MessageState mstate,
					const String& parname1, const String& par1,
					const String& parname2, const String& par2
		);
	inline void AddDebugMessage(const String& description, const MessageState mstate,
					const String& parname1, const String& par1,
					const String& parname2, const String& par2,
					const String& parname3, const String& par3
		);
	inline void AddDebugMessage(const String& description, const MessageState mstate,
					const String& parname1, const String& par1,
					const String& parname2, const String& par2,
					const String& parname3, const String& par3,
					const String& parname4, const String& par4
		);
	inline void AddDebugMessage(const String& description, const MessageState mstate,
					const String& parname1, const String& par1,
					const String& parname2, const String& par2,
					const String& parname3, const String& par3,
					const String& parname4, const String& par4,
					const String& parname5, const String& par5
		);
	inline void AddDebugMessage(const String& description, const MessageState mstate,
					const String& parname1, const String& par1,
					const String& parname2, const String& par2,
					const String& parname3, const String& par3,
					const String& parname4, const String& par4,
					const String& parname5, const String& par5,
					const String& parname6, const String& par6
		);
	inline void AddDebugMessage(const String& description, const MessageState mstate,
					const String& parname1, const String& par1,
					const String& parname2, const String& par2,
					const String& parname3, const String& par3,
					const String& parname4, const String& par4,
					const String& parname5, const String& par5,
					const String& parname6, const String& par6,
					const String& parname7, const String& par7
		);
	inline void AddMessageRegistrator(MessageRegistrator* messageregistrator);
	inline void RemoveMessageRegistrator();
private:
	MessageRegistrator* msreg_m{nullptr};
};


inline void Registrator::AddMessage(const String& description, const MessageState mstate,
				TStringList* param) {
	if(msreg_m) {
		msreg_m->AddMessage(description, mstate, param);
	}
}

inline void Registrator::Status(const String& message) {
	if(msreg_m) {
		msreg_m->Status(message);
	}
}

inline void Registrator::AddError(const String& description) {
	if(msreg_m) {
		msreg_m->AddError(description);
	}
}

inline void Registrator::AddError(const String& description,
				const String& parname1, const String& par1) {
	if(msreg_m) {
		msreg_m->AddError(description, parname1, par1);
	}
}

inline void Registrator::AddError(const String& description,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2) {
	if(msreg_m) {
		msreg_m->AddError(description, parname1, par1,
					parname2, par2);
	}
}

inline void Registrator::AddError(const String& description,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2,
				const String& parname3, const String& par3) {
	if(msreg_m) {
		msreg_m->AddError(description,
					parname1, par1,
					parname2, par2,
					parname3, par3);
	}
}

inline void Registrator::AddError(const String& description,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2,
				const String& parname3, const String& par3,
				const String& parname4, const String& par4) {
	if(msreg_m) {
		msreg_m->AddError(description,
					parname1, par1,
					parname2, par2,
					parname3, par3,
					parname4, par4);
	}
}

inline void Registrator::AddError(const String& description,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2,
				const String& parname3, const String& par3,
				const String& parname4, const String& par4,
				const String& parname5, const String& par5) {
	if(msreg_m) {
		msreg_m->AddError(description,
					parname1, par1,
					parname2, par2,
					parname3, par3,
					parname4, par4,
					parname5, par5);
	}
}

inline void Registrator::AddError(const String& description,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2,
				const String& parname3, const String& par3,
				const String& parname4, const String& par4,
				const String& parname5, const String& par5,
				const String& parname6, const String& par6) {
	if(msreg_m) {
		msreg_m->AddError(description,
					parname1, par1,
					parname2, par2,
					parname3, par3,
					parname4, par4,
					parname5, par5,
					parname6, par6);
	}
}

inline void Registrator::AddError(const String& description,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2,
				const String& parname3, const String& par3,
				const String& parname4, const String& par4,
				const String& parname5, const String& par5,
				const String& parname6, const String& par6,
				const String& parname7, const String& par7) {
	if(msreg_m) {
		msreg_m->AddError(description,
					parname1, par1,
					parname2, par2,
					parname3, par3,
					parname4, par4,
					parname5, par5,
					parname6, par6,
					parname7, par7);
	}
}

inline void Registrator::AddMessage_(const String& description, const MessageState mstate,
				const String& parname1, const String& par1) {
	if(msreg_m) {
		msreg_m->AddMessage_(description, mstate, parname1, par1);
	}
}

inline void Registrator::AddMessage_(const String& description, const MessageState mstate,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2) {
	if(msreg_m) {
		msreg_m->AddMessage_(description, mstate,
					parname1, par1,
					parname2, par2);
	}
}

inline void Registrator::AddMessage_(const String& description, const MessageState mstate,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2,
				const String& parname3, const String& par3) {
	if(msreg_m) {
		msreg_m->AddMessage_(description, mstate,
					parname1, par1,
					parname2, par2,
					parname3, par3);
	}
}

inline void Registrator::AddMessage_(const String& description, const MessageState mstate,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2,
				const String& parname3, const String& par3,
				const String& parname4, const String& par4) {
	if(msreg_m) {
		msreg_m->AddMessage_(description, mstate,
					parname1, par1,
					parname2, par2,
					parname3, par3,
					parname4, par4);
	}
}

inline void Registrator::AddMessage_(const String& description, const MessageState mstate,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2,
				const String& parname3, const String& par3,
				const String& parname4, const String& par4,
				const String& parname5, const String& par5) {
	if(msreg_m) {
		msreg_m->AddMessage_(description, mstate,
					parname1, par1,
					parname2, par2,
					parname3, par3,
					parname4, par4,
					parname5, par5);
	}
}

inline void Registrator::AddMessage_(const String& description, const MessageState mstate,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2,
				const String& parname3, const String& par3,
				const String& parname4, const String& par4,
				const String& parname5, const String& par5,
				const String& parname6, const String& par6) {
	if(msreg_m) {
		msreg_m->AddMessage_(description, mstate,
					parname1, par1,
					parname2, par2,
					parname3, par3,
					parname4, par4,
					parname5, par5,
					parname6, par6);
	}
}

inline void Registrator::AddMessage_(const String& description, const MessageState mstate,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2,
				const String& parname3, const String& par3,
				const String& parname4, const String& par4,
				const String& parname5, const String& par5,
				const String& parname6, const String& par6,
				const String& parname7, const String& par7) {
	if(msreg_m) {
		msreg_m->AddMessage_(description, mstate,
					parname1, par1,
					parname2, par2,
					parname3, par3,
					parname4, par4,
					parname5, par5,
					parname6, par6,
					parname7, par7);
	}
}

inline void Registrator::AddDebugMessage(const String& description, const MessageState mstate) {
	if(msreg_m) {
		msreg_m->AddDebugMessage(description, mstate);
	}
}

inline void Registrator::AddDebugMessage(const String& description, const MessageState mstate,
				const String& parname1, const String& par1
	) {

}

inline void Registrator::AddDebugMessage(const String& description, const MessageState mstate,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2) {
	if(msreg_m) {
		msreg_m->AddDebugMessage(description, mstate,
					parname1, par1,
					parname2, par2);
	}
}

inline void Registrator::AddDebugMessage(const String& description, const MessageState mstate,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2,
				const String& parname3, const String& par3) {
	if(msreg_m) {
		msreg_m->AddDebugMessage(description, mstate,
					parname1, par1,
					parname2, par2,
					parname3, par3);
	}
}

inline void Registrator::AddDebugMessage(const String& description, const MessageState mstate,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2,
				const String& parname3, const String& par3,
				const String& parname4, const String& par4) {
	if(msreg_m) {
		msreg_m->AddDebugMessage(description, mstate,
					parname1, par1,
					parname2, par2,
					parname3, par3,
					parname4, par4);
	}
}

inline void Registrator::AddDebugMessage(const String& description, const MessageState mstate,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2,
				const String& parname3, const String& par3,
				const String& parname4, const String& par4,
				const String& parname5, const String& par5) {
	if(msreg_m) {
		msreg_m->AddDebugMessage(description, mstate,
					parname1, par1,
					parname2, par2,
					parname3, par3,
					parname4, par4,
					parname5, par5);
	}
}

inline void Registrator::AddDebugMessage(const String& description, const MessageState mstate,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2,
				const String& parname3, const String& par3,
				const String& parname4, const String& par4,
				const String& parname5, const String& par5,
				const String& parname6, const String& par6) {
	if(msreg_m) {
		msreg_m->AddDebugMessage(description, mstate,
					parname1, par1,
					parname2, par2,
					parname3, par3,
					parname4, par4,
					parname5, par5,
					parname6, par6);
	}
}

inline void Registrator::AddDebugMessage(const String& description, const MessageState mstate,
				const String& parname1, const String& par1,
				const String& parname2, const String& par2,
				const String& parname3, const String& par3,
				const String& parname4, const String& par4,
				const String& parname5, const String& par5,
				const String& parname6, const String& par6,
				const String& parname7, const String& par7) {
	if(msreg_m) {
		msreg_m->AddDebugMessage(description, mstate,
					parname1, par1,
					parname2, par2,
					parname3, par3,
					parname4, par4,
					parname5, par5,
					parname6, par6,
					parname7, par7);
	}
}

inline void Registrator::AddMessageRegistrator(MessageRegistrator* messageregistrator) {
	msreg_m = messageregistrator;
}

inline void Registrator::RemoveMessageRegistrator(){
	msreg_m = nullptr;
}

#endif

