    UPDATE
      (SELECT * FROM Arranjo) sel
    SET
      sel.intTipoArranjoId = [p_TipoArranjoId],  
      sel.strDescricao = [p_Descricao],
      sel.strExtId = [p_ExtId],
      sel.bolTipico = [p_Tipico],
      sel.fltResTerra = [p_ResTerra],
      sel.fltVmin = [p_Vmin],
      sel.fltVmax = [p_Vmax],
      sel.intColorId = [p_ColorId],
      sel.intStatus = [p_Status],
      sel.intNumCabos = [p_NumCabos],
      sel.intTipoFaseId = [p_TipoFase],
      sel.dttVersao = IIF(ISNULL( [p_DataVersao] ), Now(), [p_DataVersao])
    WHERE
      sel.intArranjoId = [p_ArranjoId];
