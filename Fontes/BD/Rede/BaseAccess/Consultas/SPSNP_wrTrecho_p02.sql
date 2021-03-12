    UPDATE
      (SELECT * FROM Trecho) sel
    SET
      sel.fltComprimento = [p_Comprimento],
      sel.intArranjoId = [p_ArranjoId]
    WHERE
      sel.intEstudoId = [p_EstudoId] AND
      sel.intTrechoId = [p_TrechoId];
