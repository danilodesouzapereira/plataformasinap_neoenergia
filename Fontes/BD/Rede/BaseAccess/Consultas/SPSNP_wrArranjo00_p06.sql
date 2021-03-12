    UPDATE
      (SELECT * FROM Arranjo_00_01) sel
    SET
      sel.intSuporteId = IIF([p_SuporteId] <> -1, [p_SuporteId], NULL),
      sel.intFixacaoItem = IIF([p_FixacaoItem1] <> -1, [p_FixacaoItem1], NULL),
      sel.intCaboId = [p_Cabo1Id]
    WHERE
      sel.intArranjoId = [p_ArranjoId] AND
      sel.intCaboItem = 1 AND
      ((sel.intTipoFaseId = [p_TipoFase1Id] ) OR ( [p_TipoArranjoId] = 1));
