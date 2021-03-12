INSERT INTO
  PerdaReguSec (
               dttData, intRedeId, fltFatPotencia, fltFatCarga, fltFatCargaLevePesada, fltFatUtilizacao, fltPotNominal, fltCarregamento, fltComprimento, 
               fltTaxaCrescimento, fltVNom_AT, fltVNom_BT, strRegional, strTipologia, strFase, strTrafo, strCabo, strCaboTronco, strCaboRamal, fltCompTronco, 
               fltCompRamal, fltCompVaoMedio, bolFlow
               )
VALUES
  (
  [p_Data], [p_RedeId], [p_FatPotencia], [p_FatCarga], [p_FatCargaLevePesada], [p_FatUtilizacao], [p_PotNominal], [p_Carregamento], [p_Comprimento], 
  [p_TaxaCrescimento], [p_VNom_AT], [p_VNom_BT], [p_Regional], [p_Tipologia], [p_Fase], [p_Trafo], [p_Cabo], [p_CaboTronco], [p_CaboRamal], [p_CompTronco], 
  [p_CompRamal], [p_CompVaoMedio], [p_Flow]
  )
  