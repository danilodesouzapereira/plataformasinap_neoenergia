//---------------------------------------------------------------------------
#ifndef VTInterfaceNIXH
#define VTInterfaceNIXH

//---------------------------------------------------------------------------
#include <Classes.hpp>
//---------------------------------------------------------------------------
struct strCFGINIX{
       bool pr_com_corrente;
       bool al_com_corrente;
       bool fd_unico_a4;
       bool fd_unico_trafo;
       int  patamar;
       double fdA4;
	   double fdTrafo;
	   bool   carregDiurno;
       };
//---------------------------------------------------------------------------
class VTApl;

//---------------------------------------------------------------------------
class VTInterfaceNIX : public TObject
   {
   public:
                   __fastcall  VTInterfaceNIX(void) {};
                   __fastcall ~VTInterfaceNIX(void) {};
         virtual bool __fastcall  ExportaArqNix(TStrings *lines) = 0;
//      virtual bool __fastcall  ExportaArqNix(TStrings *lines,
//                                                bool pr_com_corrente=false,
//                                                bool al_com_corrente=false,
//                                                int  patamar = -1) = 0;
   public: //property
      __property bool   is_PR_ComCorrente = {read  = cfg.pr_com_corrente,
                                             write = cfg.pr_com_corrente};
      __property bool   is_AL_ComCorrente = {read  = cfg.al_com_corrente,
                                             write = cfg.al_com_corrente};
      __property bool   is_FD_UnicoA4     = {read  = cfg.fd_unico_a4,
                                             write = cfg.fd_unico_a4};
      __property bool   is_FD_UnicoTrafo  = {read  = cfg.fd_unico_trafo,
                                             write = cfg.fd_unico_trafo};
      __property int    Patamar           = {read  = cfg.patamar,
                                             write = cfg.patamar};
      __property double FD_A4             = {read  = cfg.fdA4,
                                             write = cfg.fdA4};
	  __property double FD_Trafo          = {read  = cfg.fdTrafo,
											 write = cfg.fdTrafo};
	  //proprio da eletropaulo...
	  __property bool   CarregamentoDirno = {read  = cfg.carregDiurno,
											 write = cfg.carregDiurno};

   protected: //dados acessados por property
   strCFGINIX   cfg;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTInterfaceNIX
//---------------------------------------------------------------------------
VTInterfaceNIX* __fastcall NewObjInterfaceNIX(VTApl *apl);
//---------------------------------------------------------------------------
#endif
//eof



 