//---------------------------------------------------------------------------
#ifndef TBDfiguraH
#define TBDfiguraH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <ADODB.hpp>
#include <map>
#include "VTBDfigura.h"
//#include "BD_inc.h"
using namespace std;
//---------------------------------------------------------------------------
class VTFiguraComentario;
class VTEstilo;
class VTEstilos;
class VTFiguras;
class VTFigura;
class VTInfoset;
class VTLink;
class VTRedes;
class VTRetangulo;
class VTObra;
class VTLog;
struct strUTM;
struct strCOORD;
class strATRIBUTO_GRAFICO;
class strESTILO_ATRIBUTO;
//---------------------------------------------------------------------------
class TBDfigura : public VTBDfigura
   {
   public:
           __fastcall  TBDfigura(VTApl *apl_owner, VTSNPDBConn *bd_conn);
		   __fastcall ~TBDfigura(void);
	  bool __fastcall  	LeAuxiliaresFiguras(void);
//      bool __fastcall  LeFiguras(VTEstudo *estudo);
	  bool __fastcall  LeFiguras(VTEstudo *estudo, bool estudo_novo, TList *lisREDE_SELE);
	  bool __fastcall	RemoveEstudo(VTEstudo *estudo);
	  bool __fastcall  SalvaFiguras(VTEstudo *estudo);

   private:  //métodos
//      bool __fastcall Conecta(void);
//	  void __fastcall Desconecta(void);
	  //metodos que tem que sair do BD
//	  void __fastcall AtualizaIndices(void);
//	  VTObra* __fastcall ExisteObra(int obra_id);
      AnsiString __fastcall IdsComentariosSQL(void);
	  bool __fastcall FiguraFilename(AnsiString &filename);
	  void __fastcall Loga(AnsiString msg);
	  int  __fastcall NextComentarioId(void);
	  void __fastcall ObtemCentroFigura(VTFigura *figura,strUTM &utm, strCOORD &esq);
      bool __fastcall SalvaAuxiliaresEstilo(void);
	  bool __fastcall SalvaEstilos(void);
	  //defaults
	  bool __fastcall LeEstilos(void);
//	  bool __fastcall LeTabelasDefault(void);
	  bool __fastcall LeDefaultAtributoGrafico(void);
	  bool __fastcall LeDefaultFormaAtributo(void);
	  bool __fastcall LeDefaultForma(void);
	  bool __fastcall LeDefaultTipoValorAtributoGrafico(void);
	  bool __fastcall PreencheEstilos(void);

	  //metodos macros
	  bool __fastcall DeleteFigura(VTFigura *figura);
	  bool __fastcall ReadComentario(bool estudo_novo);
//      bool __fastcall ReadComentarioTexto(void);
	  bool __fastcall ReadComentarioLink(bool estudo_novo);
	  bool __fastcall ReadRetangulo(bool estudo_novo);
//      bool __fastcall ReadTexto(VTFigura *figura);
	  bool __fastcall WriteComentario(VTEstudo *estudo);
	  bool __fastcall WriteEstilo(VTEstilo *estilo);
	  bool __fastcall WriteEstiloAtributo(VTEstilo *estilo);
	  bool __fastcall WriteFigura(VTFigura *figura);
	  bool __fastcall WriteLink(VTFiguraComentario *comentario);
	  bool __fastcall WriteRetangulo(VTEstudo *estudo);
	  bool __fastcall WriteRetangulo(VTFiguraComentario *comentario);
      bool __fastcall WriteRetangulo(VTRetangulo *retangulo);
	  bool __fastcall WriteTexto(VTFiguraComentario *comentario);
	  //funcoes para apagar os registros
	  bool __fastcall SPSNP_dlAllEstiloAtributo(int estilo_id);
	  bool __fastcall SPSNP_dlAllEstiloAtributoFiguraEstudo(int estudo_id);
	  bool __fastcall SPSNP_dlAllEstiloAtributoLinkEstudo(int estudo_id);
	  bool __fastcall SPSNP_dlAllEstiloAtributoTextoEstudo(int estudo_id);
	  bool __fastcall SPSNP_dlAllEstiloEstudo(int estudo_id);
	  bool __fastcall SPSNP_dlAllFiguraEstudo(int estudo_id);
	  bool __fastcall SPSNP_dlAllLink(int figuraId);
	  bool __fastcall SPSNP_dlAllLinkEstudo(int estudo_id);
	  bool __fastcall SPSNP_dlAllTexto(int figuraId);
	  bool __fastcall SPSNP_dlAllTextoEstudo(int estudo_id);
	  bool __fastcall SPSNP_dlAllValorFormaAtributo(int figuraId);
	  bool __fastcall SPSNP_dlAllValorFormaAtributoEstudo(int estudo_id);
	  bool __fastcall SPSNP_dlEstiloAtributo(int estiloAtributo_id);
	  bool __fastcall SPSNP_dlFigura(int figuraId);
//	  bool __fastcall SPSNP_dlLink(int linkId);
	  //funcoes para escrever inserts
	  bool __fastcall SPSNP_wrAtributoGraficoIns(strATRIBUTO_GRAFICO *str);
	  bool __fastcall SPSNP_wrEstiloIns(VTEstilo *estilo, int force_id = -1);
	  bool __fastcall SPSNP_wrEstiloAtributoIns(strESTILO_ATRIBUTO *str);
	  bool __fastcall SPSNP_wrFiguraIns(VTFigura *figura);
	  bool __fastcall SPSNP_wrFormaAtributoIns(strFORMA_ATRIBUTO *str);
	  bool __fastcall SPSNP_wrFormaIns(strAUXILIAR_FIGURA *aux);
	  bool __fastcall SPSNP_wrLinkIns(VTFigura *figura, VTLink *link);
	  bool __fastcall SPSNP_wrTextoIns(strTEXTO *str);
	  bool __fastcall SPSNP_wrTipoValorAtributGraficoIns(strAUXILIAR_FIGURA *aux);
	  bool __fastcall SPSNP_wrValorFormaAtributoIns(strVALOR_FORMA_ATRIBUTO *str);
//	  bool __fastcall SPSNP_wrValorFormaAtributoUpd(strVALOR_FORMA_ATRIBUTO *str);
	  //funcoes para escrever updates
	  bool __fastcall SPSNP_wrFiguraUpd(VTFigura *figura);
	  bool __fastcall SPSNP_wrLinkUpd(VTFigura *figura, VTLink *link);
	  //funcoes para inserir os parametros
	  bool __fastcall SPSNP_rdTextoCursor(VTFigura *figura, TADOQuery* ptrQryDS);

	  //funcoes para buscar id novo
	  int  __fastcall QueryNextId(void);
//	  int  __fastcall QueryNextId(AnsiString tabela, AnsiString coluna);
//	  int  __fastcall FNSNP_rdAtributoGraficoIdProx(void);
//	  int  __fastcall FNSNP_rdEstiloIdProx(void);
//	  int  __fastcall FNSNP_rdEstiloAtributoIdProx(void);
//	  int  __fastcall FNSNP_rdFiguraIdProx(void);
//	  int  __fastcall FNSNP_rdFormaAtributoIdProx(void);
//	  int  __fastcall FNSNP_rdFormaIdProx(void);
//	  int  __fastcall FNSNP_rdLinkIdProx(void);
//	  int  __fastcall FNSNP_rdTextoIdProx(void);
//	  int  __fastcall FNSNP_rdTipoValorAtributoGraficoIdProx(void);
//	  int  __fastcall FNSNP_rdValorFormaAtributoIdProx(void);


   private:  //objetos externos
      VTApl       *apl;
	  VTFiguras   *figuras;
	  VTEstilos   *estilos;
      VTInfoset   *infoset;
      VTRedes     *redes;
	  VTSNPDBConn *bd_conn;
	  VTEstudo *estudo;
	  VTLog *plog; // Ponteiro para objeto de log

   private:  //dados locais
	  struct{
			int comentario;
			int link;
			}next_id;
	  TList *lisOBRA;
	  TList *lisREDES;
	  //strings para logar erro
	  AnsiString mErrorFunction;
	  AnsiString mErrorMsg;
   };

#endif
//---------------------------------------------------------------------------
//eof