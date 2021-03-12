    UPDATE
      (SELECT * FROM Arranjo_00_01) sel
    SET
      sel.intSuporteId = IIF([p_SuporteId] <> -1, [p_SuporteId], NULL),
      sel.intFixacaoItem = IIF([p_FixacaoItem] <> -1, [p_FixacaoItem], NULL),
      sel.intCaboId = [p_CaboId]
    WHERE
      sel.intArranjoId = [p_ArranjoId];
