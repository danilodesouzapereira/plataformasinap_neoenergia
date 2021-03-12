  INSERT INTO
    EquipamentoBarra ( intBarraId, fltPosBar, intDx, intDy, intEstudoId, intEquipId )
  VALUES
    ( [p_BarraId1], [p_PosBar1], IIF([p_Dx] <> -1, [p_Dx], NULL), IIF([p_Dy] <> -1, [p_Dy], NULL), [p_EstudoId], [p_EquipId] );
