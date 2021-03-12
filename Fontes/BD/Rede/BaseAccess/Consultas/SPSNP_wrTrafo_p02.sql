    UPDATE
      (SELECT * FROM Trafo) sel
    SET
      sel.bolAutoTrafo   = [p_AutoTrafo],
      sel.fltPerdaFerro  = [p_PerdaFerro],
      sel.intLtcTipoUtl  = [p_LtcTipoUtl],
      sel.intLtcBarRefId = [p_LtcBarRefId],
      sel.fltLtcVpu      = [p_LtcVpu],
      sel.fltZ0R         = [p_Z0R],
      sel.fltZ0X         = [p_Z0X],
      sel.fltZ1R         = [p_Z1R],
      sel.fltZ1X         = [p_Z1X]
    WHERE
      sel.intEstudoId = [p_EstudoId] AND
      sel.intTrafoId = [p_TrafoId];
