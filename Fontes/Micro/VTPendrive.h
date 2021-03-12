//---------------------------------------------------------------------------
#ifndef VTPenDriveH
#define VTPenDriveH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTPendrive : public TObject
   {
   public:  //property
      __property AnsiString Id         = {read=PD.m_strPNPDeviceID,  write=PD.m_strPNPDeviceID};
      __property AnsiString DeviceID   = {read=PD.m_strDeviceID,     write=PD.m_strDeviceID};
      __property AnsiString DeviceName = {read=PD.m_strDeviceName,   write=PD.m_strDeviceName};
      __property AnsiString DiskUnit   = {read=PD.m_strDiskUnit,     write=PD.m_strDiskUnit};
      __property AnsiString Status     = {read=PD.m_strStatus,       write=PD.m_strStatus};

   public:
              __fastcall VTPendrive(void) {};
      virtual __fastcall ~VTPendrive(void) {};

   protected: //dados acessados via property
      struct   {
               AnsiString m_strPNPDeviceID;   // Identificador do dispositivo no sistema
               AnsiString m_strDeviceID;      // Identificador do disco no sistema
               AnsiString m_strDiskUnit;      // Unidade lógica
               AnsiString m_strDeviceName;    // Nome do pendrive
               AnsiString m_strStatus;        // Status do dispositivo
               } PD;
   };


#endif
//---------------------------------------------------------------------------
//eof

