    UPDATE
      (SELECT * FROM Chave) sel
    SET
      sel.intTipoChaveId = [p_TipoChaveId],
      sel.intEstado = [p_Estado],
      sel.fltInom = [p_Inom],
      sel.bolTelecomando = [p_Telecomando],
      sel.bolProtecao = [p_Protecao],
      sel.intTecnoCom = [p_TecnoCom]
    WHERE
      sel.intEstudoId = [p_EstudoId] AND
      sel.intChaveId = [p_ChaveId];
