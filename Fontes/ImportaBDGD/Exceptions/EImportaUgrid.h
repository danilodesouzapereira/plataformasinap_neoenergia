//---------------------------------------------------------------------------

#ifndef EImportaUgridH
#define EImportaUgridH
// ---------------------------------------------------------------------------------------- includes
#include <Classes.hpp>
// ----------------------------------------------------------------------------------------
class EImportaUgrid : public Sysutils::Exception
{
	public:

		//Construtor e Destrutor
		__fastcall  EImportaUgrid(AnsiString msg);

};
class EImportaUgridDAO : public EImportaUgrid
{
	public:

		//Construtor e Destrutor
		__fastcall  EImportaUgridDAO(AnsiString msg);

};
#endif
// --------------------------------------------------------------------------------------------- eof

