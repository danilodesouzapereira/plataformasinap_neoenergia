    UPDATE
      (SELECT * FROM Mutua) sel
    SET
      sel.intTrecho1Id = [p_Trecho1Id],
      sel.intTrecho1BarraSntId = [p_Trecho1BarraSntId],
      sel.fltTrecho1Pos = [p_Trecho1Pos],
      sel.intTrecho2Id = [p_Trecho2Id],
      sel.intTrecho2BarraSntId = [p_Trecho2BarraSntId],
      sel.fltTrecho2Pos = [p_Trecho2Pos],
      sel.fltZ0R = [p_Z0R],
      sel.fltZ0X = [p_Z0X],
      sel.fltZ1R = [p_Z1R],
      sel.fltZ1X = [p_Z1X]
    WHERE
      sel.intEstudoId = [p_EstudoId] AND
      sel.intMutuaId = [p_MutuaId];
