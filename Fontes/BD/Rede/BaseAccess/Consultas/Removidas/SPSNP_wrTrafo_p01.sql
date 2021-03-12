    INSERT INTO
      Trafo (bolAutoTrafo, fltPerdaFerro, intLtcTipoUtl, intLtcBarRefId, fltLtcVpu, fltZ0R, fltZ0X, fltZ1R, fltZ1X, intEstudoId, intTrafoId)
    VALUES
      ( [p_AutoTrafo], [p_PerdaFerro], [p_LtcTipoUtl], IIF([p_LtcBarRefId] <> -1, [p_LtcBarRefId], NULL), [p_LtcVpu], [p_Z0R], [p_Z0X], [p_Z1R], [p_Z1X], [p_EstudoId], [p_TrafoId] );
