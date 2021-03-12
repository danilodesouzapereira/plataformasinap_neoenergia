//---------------------------------------------------------------------------
#ifndef TDiagRegH
#define TDiagRegH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.CheckLst.hpp>
#include "VTDiagEqpto.h"
//---------------------------------------------------------------------------
//opções para diagnostico de Regulador
enum opDIAG_REG {
                    opRegBARREF = 0,
                    opRegCOUNT
                };
//texto referente às opções
const AnsiString op_REG_TEXT [opRegCOUNT] = {
                                             "Barra de Referência não conectada"
                                            };
//---------------------------------------------------------------------------
//colunas do log
enum col_LOG_REG {
                     colReg_SEGMENTO=0,
                     colReg_REDE,
                     colReg_EQPTO,
                     colReg_CODIGO,
                     colReg_TIPOERRO,
                     colReg_CODBARRA1,
                     colReg_CODBARRA2,
//                     colReg_CODBARRA3,
//                     colReg_CODSENTIDO,
                     colReg_CODBARREF,
                     colReg_CODBARREFINV,
                     colReg_DESCRICAO,
                     colReg_COUNT};
//---------------------------------------------------------------------------
class VTRegulador;
//class TFormDiagEqptoBarra;
//---------------------------------------------------------------------------
class DIAG_REG : public TObject
   {
   public:
       int        tipo_rede;
       AnsiString nome_tipo;
   };
//---------------------------------------------------------------------------
class TDiagReg : public VTDiagEqpto
{
public:         //User declarations
                __fastcall TDiagReg(VTApl *apl);
                __fastcall ~TDiagReg(void);
    void        __fastcall Inicia(void);
    void        __fastcall IniciaLog(TStringList *log);
    bool        __fastcall Executa(void);
    bool        __fastcall LeArquivo(void);
    bool        __fastcall SalvaArquivo(void);
    void        __fastcall ShowFormCfg(void);
    int         __fastcall ShowFormCfgModal(void);
    void        __fastcall PreencheListView(TListView *listView);
    int         __fastcall Tipo(void);
    AnsiString  __fastcall TipoAsString(void);
    void        __fastcall IniciaCheckList(TCheckListBox *checkListBox);
    void        __fastcall LeCheckList(TCheckListBox *checkListBox);

public:         //Dados internos
                /* TODO : mudar pra dados acessados por property */
                //TList       *LisCfgDiagReg;
    TStringList *log_reg;
    struct
    {
        long  n_total;
        long  n_verif;
        long  n_ocorrencias;
    } infoReg;

public:         //property
    __property bool OpcaoDiagBarRef = {read=dgReg.barref, write=dgReg.barref};

private:        //User declarations
    void        __fastcall AddLinhaLog(VTRegulador *reg, int opDIAG_REG);
//    DIAG_REG*   __fastcall CriaCfgRegDefault(int redeSEGMENTO, AnsiString nomeTipo);
//    bool        __fastcall ExisteCfgTipoRede(int redeSEGMENTO);
//    void        __fastcall VerificaTiposRede(void);
    void        __fastcall VerificaReg(VTRegulador *reg);

private:        //Dados externos
    VTApl *apl;

private:        //Dados internos
                //TFormDiagEqptoReg *formDiagEqptoReg;

protected:      //dados acessados por property
    struct
    {
        bool    barref;
    } dgReg;

protected:      //métodos acessados via property
    long         __fastcall PM_GetItensVerificados(void);
    TStringList* __fastcall PM_GetLog(void);
    long         __fastcall PM_GetOcorrencias(void);
};
//---------------------------------------------------------------------------
//eof
#endif
