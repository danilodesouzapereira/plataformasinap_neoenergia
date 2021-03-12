INSERT INTO
  PerdaReguEtd (
               dttData, intRedeId, fltFatCarga, fltFatUtilizacao, intTipoId, fltPotInstalada, 
               fltDemandaMax, intNumTrafo, intNumTrafoMin, intNumTrafoMax, intNumAlimMin, 
               intNumAlimMax, strAlim, fltTaxaCrescimento, fltVNom_AT, fltVNom_BT, bolFlow
               )
VALUES
  (
  [p_Data], [p_RedeId], [p_FatCarga], [p_FatUtilizacao], [p_TipoId], [p_PotInstalada], 
  [p_DemandaMax], [p_NumTrafo], [p_NumTrafoMin], [p_NumTrafoMax], [p_NumAlimMin], 
  [p_NumAlimMax], [p_Alim], [p_TaxaCrescimento], [p_VNom_AT], [p_VNom_BT], [p_Flow]
  )
  