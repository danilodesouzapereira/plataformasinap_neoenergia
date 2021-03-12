    UPDATE
      (SELECT * FROM Carga) sel
    SET
      sel.intTipoFaseId     = [p_TipoFaseId],
      sel.intTipoConsId     = [p_TipoConsId],
      sel.fltEnergia        = [p_Energia],
      sel.fltIctepu         = [p_ICtePu], 
      sel.fltZctepu         = [p_ZCtePu], 
      sel.fltSctepu         = [p_SCtePu], 
      sel.intNumConsRes     = [p_NumConsRes], 
      sel.intNumConsCom     = [p_NumConsCom], 
      sel.intNumConsInd     = [p_NumConsInd], 
      sel.intNumConsRur     = [p_NumConsRur], 
      sel.intNumConsOut     = [p_NumConsOut], 
      sel.intNumConsA4      = [p_NumConsA4]
    WHERE
      sel.intEstudoId = [p_EstudoId] AND
      sel.intCargaId = [p_CargaId];

      
      