      UPDATE
        (SELECT * FROM Arranjo_00_01) sel
      SET
        sel.intSuporteId = [p_SuporteId],
        sel.intFixacaoItem = [p_FixacaoItem3],
        sel.intCaboId = [p_Cabo3Id]
      WHERE
        sel.intArranjoId = [p_ArranjoId] AND
        sel.intCaboItem = 3 AND
        sel.intTipoFaseId = [p_TipoFase3Id];
