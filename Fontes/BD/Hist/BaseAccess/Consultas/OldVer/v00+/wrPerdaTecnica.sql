INSERT INTO
  PerdaTecnica (
               intRedeId, dttData,
               fltEnergiaSuprimento, fltEnergiaGerador, fltEnergiaRecebida, fltEnergiaEntregue, fltEnergiaCarga, fltEnergiaPerdaFluxo, fltEnergiaPerdaTotal,
               fltPerdaCabo, fltPerdaTrafoCu, fltPerdaTrafoFe, fltPerdaCapacitor, fltPerdaReator, fltPerdaRegulador, fltPerdaMedidor, fltPerdaRamal, fltPerdaOutros,
               intQtdeTrafo, intQtdeCabo, intQtdeCapacitor, intQtdeReator, intQtdeRegulador, intQtdeConsA4, intQtdeConsRes, intQtdeConsCom, intQtdeConsInd, intQtdeConsRur, intQtdeConsOut
               )
VALUES
  (
  [p_RedeId], [p_Data],
  [p_EnergiaSuprimento], [p_EnergiaGerador], [p_EnergiaRecebida], [p_EnergiaEntregue], [p_EnergiaCarga], [p_EnergiaPerdaFluxo], [p_EnergiaPerdaTotal],
  [p_PerdaCabo], [p_PerdaTrafoCobre], [p_PerdaTrafoFerro], [p_PerdaCapacitor], [p_PerdaReator], [p_PerdaRegulador], [p_PerdaMEdidor], [p_PerdaRamal], [p_PerdaOutros],
  [p_QtdeTrafo], [p_QtdeCabo], [p_QtdeCpacitor], [p_QtdeReator], [p_QtdeRegulador], [p_QtdeConsA4], [p_QtdeConsRes], [p_QtdeConsCom], [p_QtdeConsInd], [p_QtdeConsRur], [p_QtdeConsOut]
  )
  