    UPDATE
      (SELECT * FROM TipoRede) sel
    SET
      sel.strCodigo    = [p_Codigo], 
      sel.strDescricao = [p_Descricao], 
      sel.fltVMin      = [p_Vmin], 
      sel.fltVMax      = [p_Vmax], 
      sel.intColorId   = [p_ColorId], 
      sel.intStatus    = [p_Status] 
    WHERE
      sel.intTipoRedeId = [p_TipoRedeId];
