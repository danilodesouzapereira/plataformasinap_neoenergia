    INSERT INTO
      Rede (intTipoRedeId, intMRedeId, bolRedutivel, bolEstatica, bolRadial,
            bolEquivalente, dttRevisao, intColorId, tmsModificacao, strModificador,
            intBarraIniId, intEsqX1, intEsqY1, intEsqX2, intEsqY2, intUtmX1, intUtmY1, intUtmX2, intUtmY2,
            intAgrupamentoId, intEstudoId, intRedeId
           )
    VALUES
      ( [p_TipoRedeId], IIF([p_MRedeId] = -1, NULL, [p_MRedeId]), [p_Redutivel], [p_Estatica], [p_Radial],
        [p_Equivalente], [p_Revisao], [p_ColorId], [p_TmsModif], [p_Modificador],
        [p_BarraIniId], [p_EsqX1], [p_EsqY1], [p_EsqX2], [p_EsqY2], [p_UtmX1], [p_UtmY1], [p_UtmX2], [p_UtmY2],
        IIF([p_AgrupamentoId] = -1, NULL, [p_AgrupamentoId]), [p_EstudoId], [p_RedeId]
      );
