  INSERT INTO
    RREquipamentoBarra ( intEstudoId, intMRedeId, intEquipId, intCorteId, intBarraId, fltPosBar, intDx, intDy )
  VALUES
    ( [p_EstudoId], [p_MRedeId], [p_EquipId], [p_CorteId], [p_BarraId1], [p_PosBar1], IIF([p_Dx] <> -1, [p_Dx], NULL), IIF([p_Dy] <> -1, [p_Dy], NULL) );
