    UPDATE
      (SELECT * FROM Cabo) sel
    SET
      sel.strDescricao = [p_Descricao],
      sel.strExtId = [p_ExtId],
      sel.fltInom = [p_Inom],
      sel.fltVmin = [p_Vmin],
      sel.fltVmax = [p_Vmax],
      sel.intStatus = [p_Status]
    WHERE
      sel.intCaboId = [p_CaboId] AND
      sel.intTipoCaboId = [p_TipoCaboId];
