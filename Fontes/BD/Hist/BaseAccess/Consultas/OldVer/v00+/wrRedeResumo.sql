INSERT INTO
  RedeResumo (
             dttDATA, intREGIONAL, intRedeEtdId, intRedePrimario, strTipoCabo, strBitolaCabo, strTecnologia,
             fltTensaoNominal, fltCorrenteNominal, fltCorrenteEfetiva, fltComprimentoTotal, fltComprimentoTronco,
             fltQuedaTensaoMax, fltQuedaTensaoMin, fltCarregamentoMax, intNumConsPrimario, intNumConsSecundario, intValido, strModeloFluxo,
             fltIMedFaseA, fltIMedFaseB, fltIMedFaseC, fltIRef_FaseA, fltIRef_FaseB, fltIRef_FaseC, fltIAju_FaseA, fltIAju_FaseB, fltIAju_FaseC, 
             fltTempo_ms
             )
VALUES
  (
  [p_Data], [p_Regional], [p_ETD], [p_Primario], [p_TipoCabo], [p_BitolaCabo], [p_Tecnologia], 
  [p_TensaoNominal], [p_CorrenteNominal], [p_CorrenteEfetiva], [p_ComprimentoTotal], [p_ComprimentoTronco], 
  [p_QuedaTensaoMax], [p_QuedaTensaoMin], [p_CargtoMaximo], [p_NumConsPrimario], [p_NumConsSecundario], [p_Valido], [p_ModeloFluxo], 
  [p_IMedFaseA], [p_IMedFaseB], [p_IMedFaseC], [p_IRefFaseA], [p_IRefFaseB], [p_IRefFaseC], [p_IAjuFaseA], [p_IAjuFaseB], [p_IAjuFaseC], 
  [p_TempoMSeg]
  )
  