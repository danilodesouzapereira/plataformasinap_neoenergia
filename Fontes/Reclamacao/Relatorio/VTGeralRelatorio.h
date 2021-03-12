#ifndef VTGeralRelatorioH
#define VTGeralRelatorioH
//---------------------------------------------------------------------------
#include <Classes.hpp>
class VTRede;
class VTTrafo;
//---------------------------------------------------------------------------
struct strElemRelatorio
{
	AnsiString  NomeMiniRet;
	AnsiString  NomeTipoRede;
	AnsiString  NomeAlimPrim;
	AnsiString  NomeCT;
	double      kVA_ins;
	double      kVA_med;
	int         Diagnostico_id;
	VTRede*     rede;
	VTTrafo*    trafo;

};
struct strMiniReticulado
{
	AnsiString  NomeMiniRet;
	AnsiString  NomeTipoRede;
	VTRede*     rede;
	TList*      LisStrRedePrimaria;
};
struct strRedePrimaria
{
	AnsiString  NomeRedePrimaria;
	TList*      LisStrElemRelatorio;
}; 
//---------------------------------------------------------------------------
#endif

