    INSERT INTO
      Regiao ( intTipoRegiaoId, strCodigo, intUtm1X, intUtm1Y, intUtm2X, intUtm2Y, intAreaGeoId, intRegiaoId )
    VALUES
      ( [p_TipoRegiaoId], 
        [p_Codigo], 
        IIF([p_Utm1X]     <> -1, [p_Utm1X],     NULL), 
        IIF([p_Utm1Y]     <> -1, [p_Utm1Y],     NULL), 
        IIF([p_Utm2X]     <> -1, [p_Utm2X],     NULL), 
        IIF([p_Utm2Y]     <> -1, [p_Utm2Y],     NULL), 
        IIF([p_AreaGeoId] <> -1, [p_AreaGeoId], NULL), 
        [p_RegiaoId] 
      );

      
      