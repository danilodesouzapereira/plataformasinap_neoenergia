      INSERT INTO
        Arranjo_00_01 (intSuporteId, intFixacaoItem, intArranjoId, intCaboId, intCaboItem, intTipoFaseId)
      VALUES
        ( IIF([p_SuporteId] <> -1, [p_SuporteId], NULL), IIF([p_FixacaoItem1] <> -1, [p_FixacaoItem1], NULL), [p_ArranjoId], [p_Cabo1Id], 1, IIF([p_TipoFase1Id] <> -1, [p_TipoFase1Id], NULL) );
