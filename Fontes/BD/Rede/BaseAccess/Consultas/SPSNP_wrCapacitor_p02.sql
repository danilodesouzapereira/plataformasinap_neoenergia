    UPDATE
      (SELECT * FROM Capacitor) sel
    SET
      sel.intTipoLigacaoId = [p_TipoLigacaoId],
      sel.fltVnom = [p_Vnom],
      sel.fltQnom = [p_Qnom]
    WHERE
      sel.intEstudoId = [p_EstudoId] AND
      sel.intCapacitorId = [p_CapacitorId];
