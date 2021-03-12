      UPDATE
        (SELECT * FROM Arranjo_00_01) sel
      SET
        sel.intSuporteId = [p_SuporteId],
        sel.intFixacaoItem = [p_FixacaoItem2],
        sel.intCaboId = [p_Cabo2Id]
      WHERE
        sel.intArranjoId = [p_ArranjoId] AND
        sel.intCaboItem = 2 AND
        sel.intTipoFaseId = [p_TipoFase2Id];
