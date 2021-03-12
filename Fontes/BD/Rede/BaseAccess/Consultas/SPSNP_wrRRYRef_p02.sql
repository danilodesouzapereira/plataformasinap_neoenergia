    UPDATE
      (SELECT * FROM RRYRef) sel
    SET
      sel.fltY0G  = [p_Y0G],
      sel.fltY0B  = [p_Y0B],
      sel.fltY1G  = [p_Y1G],
      sel.fltY1B  = [p_Y1B],
      sel.blbMatY = [p_MatY]
    WHERE
      sel.intEstudoId = [p_EstudoId] AND
      sel.intMRedeId = [p_MRedeId] AND
      sel.intRRYRefId = [p_RRYRefId] AND
      sel.intCorteId = [p_CorteId];
