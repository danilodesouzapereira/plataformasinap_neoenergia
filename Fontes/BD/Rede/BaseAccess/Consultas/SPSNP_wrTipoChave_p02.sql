    UPDATE
      (SELECT * FROM TipoChave) sel
    SET
      sel.strDescricao = [p_Descricao],
      sel.intStatus    = [p_Status],
      sel.intCorId     = [p_CorId]
    WHERE
      sel.intTipoChaveId = [p_TipoChaveId];
