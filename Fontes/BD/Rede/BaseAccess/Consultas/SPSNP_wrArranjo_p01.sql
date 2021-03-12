    INSERT INTO
      Arranjo ( intTipoArranjoId, strDescricao, strExtId, bolTipico, fltResTerra, fltVmin, fltVmax, intColorId, intStatus, intNumCabos, intTipoFaseId, dttVersao, intArranjoId )
    VALUES
      ( [p_TipoArranjoId], [p_Descricao], [p_ExtId], [p_Tipico], [p_ResTerra], [p_Vmin], [p_Vmax], [p_ColorId], [p_Status], [p_NumCabos], [p_TipoFase], IIF(ISNULL([p_DataVersao]), Now(), [p_DataVersao]), [p_ArranjoId] );
