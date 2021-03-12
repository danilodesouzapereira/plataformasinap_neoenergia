    UPDATE
      (SELECT * FROM Reator) sel
    SET
      sel.intTipoLigacaoId = [p_TipoLigacaoId],
      sel.fltVnom = [p_Vnom],
      sel.fltQnom = [p_Qnom]
    WHERE
      sel.intEstudoId = [p_EstudoId] AND
      sel.intReatorId = [p_ReatorId];
