      UPDATE
        (SELECT * FROM Arranjo_00_01) sel
      SET
        sel.intSuporteId = [p_SuporteId],
        sel.intFixacaoItem = [p_FixacaoItem4],
        sel.intCaboId = [p_Cabo4Id]
      WHERE
        sel.intArranjoId = [p_ArranjoId] AND
        sel.intCaboItem = 4 AND
        sel.intTipoFaseId = [p_TipoFase4Id];
