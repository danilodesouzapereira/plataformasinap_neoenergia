    UPDATE
      (SELECT * FROM CargaIP) sel
    SET
      sel.intTipoFaseId = [p_TipoFaseId],
      sel.fltPnom = [p_Pnom],
      sel.fltFatorPot = [p_FatorPot],
      sel.intNumLamp = [p_NumLamp],
      sel.fltICtepu = [p_ICtePu],
      sel.fltZCtepu = [p_ZCtePu],
      sel.fltSCtepu = [p_SCtePu]
    WHERE
      sel.intEstudoId = [p_EstudoId] AND
      sel.intCargaIPId = [p_CargaIPId];
