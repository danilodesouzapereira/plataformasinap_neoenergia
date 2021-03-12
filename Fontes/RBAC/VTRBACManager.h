#ifndef VTRBACManagerH
#define VTRBACManagerH

// arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/index.cfm#/tasks/14642326
enum operacaoID {
                opVIEW               = 001,
                opBASICO             = 100,
                opDEVELOPER          = 101,
                opREGULADOR          = 102,
                opCONTINGENCIAS      = 201,
                opCONTINUIDADE       = 202,
                opCURTO              = 203,
                opMICROGRIDS         = 204,
                opMOTOR              = 205,
                opREDE               = 206,
                opREGULATORIA        = 207,
                opVTCD               = 208,
                opCONFIABMALHA       = 209,
                opCONSTANTES         = 301,
                opREATIVOS           = 302,
                opESTABILIDADE       = 303,
                opCARGAS             = 304,
                opHARMONICO          = 305,
                opMOTORCALCULO       = 306
                };

enum moduloID {
                modALOCART           = 1,
                modANALISECONT       = 2,
                modCONFIABILIDADE    = 3,
                modALOCACHAVE        = 4,
                modALOCATRIPOLAR     = 5,
                modENUMRELIGADOR     = 6,
                modCONTINUIDADE      = 7,
                modCURTO             = 8,
                modMULTFALTA         = 9,
                modMICROGRID         = 10,
                modPARTIDAMOTOR      = 11,
                modBALANCEAMENTO     = 12,
                modPERDATECLOTE      = 13,
                modPERDATEC          = 14,
                modPERDATECREL       = 15,
                modABERTURAFASE      = 16,
                modCAMINHOELETRICO   = 17,
                modDIAGEQUIPAMENTO   = 18,
                modDIAGFLUXO         = 19,
                modFLUXOLOTE         = 20,
                modFLUXO             = 21,
                modDIAGTECNICO       = 22,
                modMANOBRAS          = 23,
                modRELFLUXO          = 24,
                modRELJUSANTE        = 25,
                modRELDEMANDA        = 26,
                modRELEQUIPAMENTO    = 27,
                modSOLICITACAO       = 28,
                modMAPAAI            = 29,
                modMAPADCLOTE        = 30,
                modMAPACC            = 31,
                modMAPADC            = 32,
                modMAPATENSAO        = 33,
                modMAPAPT            = 34,
                modMAPAFP            = 35,
                modOCORRENCIA        = 36,
                modOTIMIZARAMAL      = 37,
                modPLANEJAMENTO      = 38,
                modANALISEREG        = 39,
                modENDERECOARIES     = 40,
                modANALISEVTCD       = 41,
                modEXPLORER          = 42,
                modFASOMETRO         = 43,
                modGRAFICO           = 44,
                modIMPORTAGIS        = 45,
                modIMPORTAGISLOTE    = 46,
                modLOCALIZAR         = 47,
                modMEDIDOR           = 48,
                modTOPOLOGIA         = 49,
                modZOOM              = 50,
                modANALISEMALHA      = 51,
                modEDITOR            = 52,
                modCABOARRANJO       = 53,
                modCURVATIPICA       = 54,
                modPATAMARES         = 55,
                modTIPOCHAVE         = 56,
                modTIPOREDE          = 57,
                modTELIN             = 58,
                modALOCABC           = 59,
                modALOCALICENCA      = 60,
                modEXIBELICENCA      = 61,
                modLIBERALICENCA     = 62,
                modRESTRINGE         = 63,
                modDEVELOPER         = 64,
                modESTABILIDADE      = 65,
                modAJUSTEDEMANDA     = 66,
                modESTIMADEMANDA     = 67,
                modMERCADO           = 68,
                modFLUXOHARMONICO    = 69,
                modMOTORCALCULO      = 70
                };

// -----------------------------------------------------------------------------
class VTApl;

// -----------------------------------------------------------------------------
class VTRBACManager : public TObject
{

public:
	__fastcall VTRBACManager(void){};
	virtual 	 __fastcall ~VTRBACManager(void){};
	virtual bool __fastcall ExistePermissaoModulo(int modulo_id) = 0;
	virtual UnicodeString __fastcall GetDescricaoOperacao(int operacao_id) = 0;
	virtual bool __fastcall SinapViewer(void) = 0;

};

// -----------------------------------------------------------------------------
// função global para criar objeto VTRBACManager
// -----------------------------------------------------------------------------
VTRBACManager* __fastcall NewObjRBACManager(VTApl *apl);

#endif
// ---------------------------------------------------------------------------
// eof
