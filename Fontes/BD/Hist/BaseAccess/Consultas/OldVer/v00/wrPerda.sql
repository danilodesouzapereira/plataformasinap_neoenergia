INSERT INTO
  Perda (
        REDE_ID, TIPOREDE_ID, SUPRIDORA_ID, DATA, CODIGO,
        ENERGIASUPRIMENTO, ENERGIAGERADOR, ENERGIARECEBIDA, ENERGIAENTREGUE, ENERGIACARGA, ENERGIAPERDAFLUXO, ENERGIAPERDATOTAL,
        PERDACABO, PERDATRAFOCOBRE, PERDATRAFOFERRO, PERDACAPACITOR, PERDAREATOR, PERDAREGULADOR, PERDAMEDIDOR, PERDARAMAL, PERDAOUTROS,
        QTDETRAFO, QTDECABO, QTDECAPACITOR, QTDEREATOR, QTDEREGULADOR, QTDECONSA4, QTDECONSRES, QTDECONSCOM, QTDECONSIND, QTDECONSRUR, QTDECONSOUT
        )
VALUES
  (
  [p_RedeId], [p_TipoRedeId], [p_SupridoraId], [p_Data], [p_Codigo], 
  [p_EnergiaSuprimento], [p_EnergiaGerador], [p_EnergiaRecebida], [p_EnergiaEntregue], [p_EnergiaCarga], [p_EnergiaPerdaFluxo], [p_EnergiaPerdaTotal],
  [p_PerdaCabo], [p_PerdaTrafoCobre], [p_PerdaTrafoFerro], [p_PerdaCapacitor], [p_PerdaReator], [p_PerdaRegulador], [p_PerdaMEdidor], [p_PerdaRamal], [p_PerdaOutros],
  [p_QtdeTrafo], [p_QtdeCabo], [p_QtdeCpacitor], [p_QtdeReator], [p_QtdeRegulador], [p_QtdeConsA4], [p_QtdeConsRes], [p_QtdeConsCom], [p_QtdeConsInd], [p_QtdeConsRur], [p_QtdeConsOut]
  )
  