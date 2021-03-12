    UPDATE
      (SELECT * FROM Equipamento) sel
    SET
      sel.intRedeId = IIF([p_RedeId] = -1, NULL, [p_RedeId]),
      sel.intTipoEquipId = [p_TipoEquipId],
      sel.bolAlteradoObra = [p_AlteradoObra],
      sel.intShow = [p_Show],
      sel.strCodigo = [p_Codigo],
      sel.strExtId = [p_ExtId],
      sel.strComentario = IIF([p_Comentario] = "", NULL, [p_Comentario]),
      sel.intDxComentario = IIF([p_Comentario] = "", NULL, [p_DxComentario]),
      sel.intDyComentario = IIF([p_Comentario] = "", NULL, [p_DyComentario])
    WHERE
      sel.intEstudoId = [p_EstudoId] AND
      sel.intEquipId = [p_EquipId];
