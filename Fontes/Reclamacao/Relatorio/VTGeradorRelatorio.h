#ifndef VTGeradorRelatorioH
#define VTGeradorRelatorioH
//---------------------------------------------------------------------------
#include <Classes.hpp>

class VTCluster;
class VTApl;
class VTPatamar;
//---------------------------------------------------------------------------
class VTGeradorRelatorio : public TObject
{
  private:

  protected:

  public:
                  __fastcall VTGeradorRelatorio(void) {};
  virtual         __fastcall ~VTGeradorRelatorio(void) {};
  virtual bool    __fastcall AbrirExcel(AnsiString FileName) = 0;
  virtual void    __fastcall Fecha(void) = 0;
  virtual bool    __fastcall InicioExcel(void) = 0;
  virtual bool    __fastcall Executar(VTCluster* cluster, TList*  ListElementos, VTPatamar *patamar) = 0;
  virtual bool    __fastcall ExecutarMINIDesligado(TList* lisCARCT, TList* lisCARPRIM) = 0;
  virtual void    __fastcall Mostra(void) = 0;
  virtual void    __fastcall Salva(AnsiString arquivo) = 0;

  virtual Variant   __fastcall DevolveSheet(int posicao) = 0; // inicia com 1 a partir do lado esquerdo
  virtual bool      __fastcall  InsereTexto(AnsiString texto, Variant Vx, TColor color, int Fila, int Coluna,
											int PosAlinHor, int PosAlinVer, double tamanho, bool Sombreado,
											bool tabelado, TColor colorInter, bool AutoFitRow = false, bool AutoFitCol = false) = 0;

  virtual bool      __fastcall  InsereTextoSimple(AnsiString texto, Variant Vx, TColor color, int Fila, int Coluna) = 0;
  virtual bool      __fastcall  Visualizar(bool visible) = 0;
};

//---------------------------------------------------------------------------
VTGeradorRelatorio* __fastcall NewObjGeradorRelatorio(VTApl* apl);
//---------------------------------------------------------------------------
#endif

