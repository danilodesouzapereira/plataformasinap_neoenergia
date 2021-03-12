//---------------------------------------------------------------------------
#ifndef VTEstudoH
#define VTEstudoH

//----------------------------------------------------------------------------
#include <Classes.hpp>

//----------------------------------------------------------------------------
class VTApl;
class VTAlternativa;
class VTObra;
class VTRede;

//----------------------------------------------------------------------------
enum enumESTUDO {estudoID_ZERO=0};
enum tipoESTUDO {estudoREDE_ORIGINAL=0, estudoPLANEJAMENTO, estudoRECLAMACAO_BT, estudoRECLAMACAO_MT};

//----------------------------------------------------------------------------
class VTEstudo : public TObject
{
public:
    __property int            Id                = {read=PD.id,                write=PD.id};
    __property int            AnoInicial        = {read=PM_GetAnoInicial,     write=PM_SetAnoInicial};
    __property int            AnoFinal          = {read=PM_GetAnoFinal,       write=PM_SetAnoFinal};
    __property int            AnoObraInicial    = {read=PM_GetAnoObraInicial};
    __property int            AnoObraFinal      = {read=PM_GetAnoObraFinal};
    __property AnsiString     Autor             = {read=PD.autor,             write=PD.autor};
    __property AnsiString     Codigo            = {read=PD.codigo,            write=PD.codigo};
    __property TDateTime      Data              = {read=PD.data,              write=PD.data};
    __property bool           Novo              = {read=PM_GetNovo,           write=PM_SetNovo};
    __property VTAlternativa* AlternativaAtiva  = {read=PM_GetAlternativaAtiva};
    __property VTAlternativa* AlternativaPadrao = {read=PM_GetAlternativaPadrao};
    __property VTObra*        ObraAtiva         = {read=PD.obra_ativa};
    __property bool           Alterado          = {read=PM_GetAlterado,       write=PM_SetAlterado};
    __property int            Tipo              = {read=PD.tipo,              write=PD.tipo};
    __property AnsiString     Justificativa     = {read=PD.justificativa,     write=PD.justificativa};
    __property int            ModalResult       = {read=PM_GetModalResult,    write=PM_SetModalResult};
    
public:
                           __fastcall  VTEstudo(void) {};
    virtual                __fastcall ~VTEstudo(void) {};
    virtual VTAlternativa* __fastcall  CopiaAlternativa(VTAlternativa *alternativa) = 0;
    virtual bool           __fastcall  DefineAlternativaAtiva(VTAlternativa *alternativa, VTObra *obra) = 0;
    virtual void           __fastcall  DefineObrasComoFixas(void) = 0;
    virtual VTAlternativa* __fastcall  ExisteAlternativa(AnsiString codigo) = 0;
    virtual VTAlternativa* __fastcall  ExisteAlternativa(int id) = 0;
    //virtual bool         __fastcall  FundeAcoesDasObras(void) = 0;
    virtual VTAlternativa* __fastcall  InsereAlternativa(AnsiString codigo, bool padrao=false) = 0;
    virtual TList*         __fastcall  LisAlternativa(void) = 0;
    virtual TList*         __fastcall  LisAlternativaExcluida(void) = 0;
    virtual void           __fastcall  LisObraEstudo(TList *lisEXT) = 0;
    virtual void           __fastcall  LisAcaoEstudo(TList *lisEXT) = 0;
    virtual void           __fastcall  LisEqptoEstudo(TList *lisEXT, VTRede *rede=NULL, int tipo=-1) = 0;
    virtual void           __fastcall  MontaRedesObrasLidasDaBase(void) = 0;
    virtual void           __fastcall  PreparaParaSalvarNaBase(void) = 0;
    virtual void           __fastcall  RetiraAlternativa(VTAlternativa *alternativa) = 0;
    virtual void           __fastcall  RetiraAlternativaInvalida(void) = 0;
    virtual void           __fastcall  RetiraObraSemAcao(void) = 0;
    virtual TStringList*   __fastcall  StrListRede(void) = 0;

protected: //métodos acessados via property
    virtual bool           __fastcall PM_GetAlterado(void) = 0;
    virtual int            __fastcall PM_GetAnoInicial(void) = 0;
    virtual int            __fastcall PM_GetAnoFinal(void) = 0;
    virtual int            __fastcall PM_GetAnoObraInicial(void) = 0;
    virtual int            __fastcall PM_GetAnoObraFinal(void) = 0;
    virtual bool           __fastcall PM_GetNovo(void) = 0;
    virtual VTAlternativa* __fastcall PM_GetAlternativaAtiva(void) = 0;
    virtual VTAlternativa* __fastcall PM_GetAlternativaPadrao(void) = 0;
    virtual int            __fastcall PM_GetModalResult(void) = 0;
    virtual void           __fastcall PM_SetAlterado(bool alterado) = 0;
//  virtual void           __fastcall PM_SetAlternativaAtiva(VTAlternativa *alternativa) = 0;
    virtual void           __fastcall PM_SetAnoInicial(int ano_ini) = 0;
    virtual void           __fastcall PM_SetAnoFinal(int ano_fim) = 0;
    virtual void           __fastcall PM_SetNovo(bool novo) = 0;
    virtual void           __fastcall PM_SetModalResult(int modal_result) = 0;

protected: //dados acessados via property
    struct
    {
        int           id;
        int           tipo;
        bool          novo;
        int           ano_ini, ano_fim;
        int             modal_result;
        AnsiString    codigo;
        AnsiString    autor;
        TDateTime     data;
        VTAlternativa *alternativa_ativa;   //alternativa ativa (ou selecionada)
        VTAlternativa *alternativa_padrao;  //alternativa padrão
        VTObra        *obra_ativa;
        AnsiString    justificativa;
    } PD;
};

//---------------------------------------------------------------------------
//função p/ criar método da classe
//---------------------------------------------------------------------------
VTEstudo* __fastcall NewObjEstudo(VTApl *apl);
//---------------------------------------------------------------------------
#endif
//eof
