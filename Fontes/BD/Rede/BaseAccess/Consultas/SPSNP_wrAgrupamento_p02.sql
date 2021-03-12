    UPDATE
      (SELECT * FROM Agrupamento) sel
    SET
      sel.strCodigo = [p_Codigo],
      sel.bolAlteradoObra = [p_AlteradoObra]
    WHERE
      sel.intAgrupamentoId = [p_AgrupamentoId] AND
      sel.intEstudoId = [p_EstudoId];
