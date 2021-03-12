    UPDATE
      (SELECT * FROM Estudo) sel
    SET
      sel.strDescricao = [p_Descricao],
      sel.strProprietario = [p_UsuarioNome],
      sel.tmsModificacao = Now(),
      sel.intTipoEstudoId = [p_TipoEstudoId],
	  sel.strJustificativa = [p_Justificativa]
    WHERE
      sel.intEstudoId = [p_EstudoId];
