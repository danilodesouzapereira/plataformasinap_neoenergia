  INSERT INTO
    EquipamentoBarra ( intEstudoId, intEquipId, intBarraId, fltPosBar, fltDx, fltDy )
  SELECT [p_EstudoIdNovo], intEquipId, intBarraId, fltPosBar, fltDx, fltDy
  FROM EquipamentoBarra WHERE intEstudoId = [p_EstudoIdOrg];
