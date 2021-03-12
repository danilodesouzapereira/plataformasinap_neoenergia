    UPDATE
      (SELECT * FROM TrafoZZ) sel
    SET
      sel.fltVnom = [p_Vnom],
      sel.fltSnom = [p_Snom],
      sel.fltZ0R = [p_Z0R],
      sel.fltZ0X = [p_Z0X]
    WHERE
      sel.intEstudoId = [p_EstudoId] AND
      sel.intTrafoZZId = [p_TrafoZZId];
