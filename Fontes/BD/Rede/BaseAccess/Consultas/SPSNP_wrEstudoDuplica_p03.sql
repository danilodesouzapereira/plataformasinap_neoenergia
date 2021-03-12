  INSERT INTO
    Equipamento (intEstudoId, intRedeId, intEquipId, intTipoEquipId, bolAlteradoObra, intShow, strCodigo, strExtId, intDxComentario, intDyComentario, strComentario, intConjCurvaId)
  SELECT [p_EstudoIdNovo], intRedeId, intEquipId, intTipoEquipId, bolAlteradoObra, intShow, strCodigo, strExtId, intDxComentario, intDyComentario, strComentario, [p_ConjCurvaIdNovo]
  FROM Equipamento WHERE intEstudoId = [p_EstudoIdOrg];
