    INSERT INTO
      Equipamento (intRedeId, intTipoEquipId, bolAlteradoObra, intShow, strCodigo, strExtId, intDxComentario, intDyComentario, strComentario, intEstudoId, intEquipId)
    VALUES
      ( IIF([p_RedeId] = -1, NULL, [p_RedeId]), [p_TipoEquipId], [p_AlteradoObra], [p_Show], [p_Codigo], IIF([p_ExtId] = "", NULL, [p_ExtId]), IIF([p_Comentario] = "", NULL, [p_Comentario]), IIF([p_Comentario] = "", NULL, [p_DxComentario]), IIF([p_Comentario] = "", NULL, [p_DyComentario]), [p_EstudoId], [p_EquipId] );
  
      
      