    INSERT INTO
      Arranjo_00_01 (intCaboItem, intTipoFaseId, intSuporteId, intFixacaoItem, intArranjoId, intCaboId)
    VALUES
      (1, [p_TipoFase], IIF([p_SuporteId] <> -1, [p_SuporteId], NULL), IIF([p_FixacaoItem] <> -1, [p_FixacaoItem], NULL), [p_ArranjoId], [p_CaboId] );
