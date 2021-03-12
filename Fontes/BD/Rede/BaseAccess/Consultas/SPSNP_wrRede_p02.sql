    UPDATE
      (SELECT * FROM Rede) sel
    SET
      sel.intTipoRedeId    = [p_TipoRedeId],
      sel.intMRedeId       = IIF([p_MRedeId] = -1, NULL, [p_MRedeId]),
      sel.bolRedutivel     = [p_Redutivel],
      sel.bolEstatica      = [p_Estatica],
      sel.bolRadial        = [p_Radial],
      sel.bolEquivalente   = [p_Equivalente],
      sel.dttRevisao       = [p_Revisao],
      sel.intColorId       = [p_ColorId],
      sel.tmsModificacao   = [p_TmsModif],
      sel.strModificador   = [p_Modificador],
      sel.intBarraIniId    = [p_BarraIniId],
      sel.intEsqX1         = [p_EsqX1],
      sel.intEsqY1         = [p_EsqY1],
      sel.intEsqX2         = [p_EsqX2],
      sel.intEsqY2         = [p_EsqY2],
      sel.intUtmX1         = [p_UtmX1],
      sel.intUtmY1         = [p_UtmY1],
      sel.intUtmX2         = [p_UtmX2],
      sel.intUtmY2         = [p_UtmY2],
      sel.intAgrupamentoId = IIF([p_AgrupamentoId] = -1, NULL, [p_AgrupamentoId])
     WHERE
       sel.intEstudoId = [p_EstudoId] AND
       sel.intRedeId = [p_RedeId];
