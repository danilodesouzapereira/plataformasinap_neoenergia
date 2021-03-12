    INSERT INTO
      Estudo ( strDescricao, strProprietario, tmsModificacao, intTipoEstudoId, intEstudoId, strJustificativa )
    VALUES
      ( [p_Descricao], [p_UsuarioNome], Now(), [p_TipoEstudoId], [p_EstudoId], [p_Justificativa] );
