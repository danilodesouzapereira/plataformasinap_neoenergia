    UPDATE
      (SELECT * FROM CapacitorSerie) sel
    SET
      sel.fltVnom = [p_Vnom],
      sel.fltSnom = [p_Snom],
      sel.fltZR = [p_ZR],
      sel.fltZX = [p_ZX]
    WHERE
      sel.intEstudoId = [p_EstudoId] AND
      sel.intCapacitorSerieId = [p_CapacitorSerieId];
