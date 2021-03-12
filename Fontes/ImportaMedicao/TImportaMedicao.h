// ---------------------------------------------------------------------------
#ifndef TImportaMedicaoH
#define TImportaMedicaoH

// arquivos incluídos---------------------------------------------------------
#include <Classes.hpp>
#include "VTImportaMedicao.h"
#include "..\Constante\Fases.h"

// ---------------------------------------------------------------------------
class VTCanal;
class VTChave;
class VTLog;
class VTPrimario;
class VTSubestacao;
class VTMedidor;
class VTMedicoes;
class VTPatamar;
class VTRadial;
class VTRede;
class strHM;

// ---------------------------------------------------------------------------
class TImportaMedicao : public VTImportaMedicao
{
public:
                  __fastcall TImportaMedicao(TComponent *Owner, VTApl *apl_owner);
                  __fastcall ~TImportaMedicao(void);
    bool          __fastcall LeArquivoCurvaPadrao(TStrings *files);

private: // métodos
    void          __fastcall CompletaDadosCanal(VTCanal *canal);
    void          __fastcall ConverteUnidade(double valor[MAX_FASE]);
    VTChave*      __fastcall ExisteChave(AnsiString cod_pri, AnsiString cod_chave);
    VTChave*      __fastcall ExisteChaveInicial(VTPrimario *primario);
    VTPrimario*   __fastcall ExistePrimario(AnsiString cod_rede_medicao);
    VTPrimario*   __fastcall ExistePrimarioCodigoRede(AnsiString cod_rede_medicao);
    VTSubestacao* __fastcall ExisteSubestacao(AnsiString cod_sub_medicao);
    int           __fastcall ExtraiCampos(AnsiString linha, AnsiString separadores);
    AnsiString    __fastcall FiltraCodigoRede(AnsiString cod_orig);
    bool          __fastcall InsereImax(double valor[MAX_FASE], double valor_max, VTChave *chave);
    void          __fastcall InsereMensagemLog(AnsiString erro);
    bool          __fastcall InsereValorFase(double valor[MAX_FASE], VTChave *chave, VTPatamar *patamar);
    bool          __fastcall InsereValorMaxFase(double valor_max, VTChave *chave, VTPatamar *patamar);
    bool          __fastcall InsereValorTri(double valor_tri, VTChave *chave, VTPatamar *patamar);
    void          __fastcall LogFecha(void);
    void          __fastcall LogInicia(void);
    void          __fastcall LogReinicia(void);
//    bool          __fastcall PrimarioValido(VTPrimario *primario);
    VTPatamar*    __fastcall PatamarDemandaMax(VTChave *chave);
    int           __fastcall TraduzTipoMedicao(AnsiString tipo_med);
    AnsiString    __fastcall TraduzTipoMedicao(int tipo_med);
    strHM         __fastcall TrataHoraInteira(strHM hora_minuto);
    bool          __fastcall ValidaValor(double valor[MAX_FASE]);
    bool          __fastcall VerificaComentario(AnsiString linha);

    // objetos externos
    VTApl       *apl_owner;
    // TList      *lisPRI_VAL; //DVK 2015.09.24
    VTLog       *log;
    VTMedicoes  *medicoes;
    VTRadial    *radial;

    // dados locais
    int         linhas_log;
    TStringList *lines; // Armazena o conteúdo do arquivo txt lido
    TStringList *campos, *hm; // Armazena uma linha com os campos de uma linha
    int         tipo_canal;
    bool        por_fase, val_max;
    TList       *lisREDE_VAL;
};

#endif

// -----------------------------------------------------------------------------
// eof
