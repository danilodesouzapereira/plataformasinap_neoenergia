INSERT INTO
  PerdaReguPri (
               dttData, intRedeId, fltFatPotencia, fltFatCarga, fltFatPerda, fltComprimento, fltDvMax, fltVNom, fltAngulo, fltDemandaMax, fltAlpha, 
               fltDEC, fltFEC, fltTaxaArbor, fltTaxaCrescVert, fltTaxaCrescHorz, fltTaxaFalhaArbor, fltTaxaFalhaEqpto, fltTaxaFalhaOutro, fltTaxaReforma, 
               fltAreaAtuacao, fltDistBaricentro, strFase, strCaboTronco, strCaboTronco_3F, strCaboTronco_2F, strCaboTronco_1F, strCaboRamal, 
               strCaboRamal_3F, strCaboRamal_2F, strCaboRamal_1F, fltCompTronco_3F, fltCompTronco_2F, fltCompTronco_1F, fltCompRamal_3F, fltCompRamal_2F, fltCompRamal_1F, 
               intNumPontoCarga, intNumConsumidor, intNumConsumidor_MT, intNumConsumidor_BT, intNumRegTensao, bolFlow
               )
VALUES
  (
  [p_Data], [p_RedeId], [p_FatPotencia], [p_FatCarga], [p_FatPerda], [p_Comprimento], [p_DvMax], [p_VNom], [p_Angulo], [p_DemandaMax], [p_Alpha], 
  [p_DEC], [p_FEC], [p_TaxaArbor], [p_TaxaCrescVert], [p_TaxaCrescHorz], [p_TaxaFalhaArbor], [p_TaxaFalhaEqpto], [p_TaxaFalhaOutro], [p_TaxaReforma], 
  [p_AreaAtuacao], [p_DistBaricentro], [p_Fase], [p_CaboTronco], [p_CaboTronco_3F], [p_CaboTronco_2F], [p_CaboTronco_1F], [p_CaboRamal], 
  [p_CaboRamal_3F], [p_CaboRamal_2F], [p_CaboRamal_1F], [p_CompTronco_3F], [p_CompTronco_2F], [p_CompTronco_1F], [p_CompRamal_3F], [p_CompRamal_2F], [p_CompRamal_1F], 
  [p_NumPontoCarga], [p_NumConsumidor], [p_NumConsumidor_MT], [p_NumConsumidor_BT], [p_NumRegTensao], [p_Flow]
  )
  