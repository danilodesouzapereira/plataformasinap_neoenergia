  INSERT INTO
    ObraAcao ( intEstudoId, intAlternativaId, intObraId, intAcaoId, intItemId, intTipoAcaoId, intEquipOriginalId, intEquipEstadoPreId, intEquipEstadoPosId )
  VALUES
    ( [p_EstudoId], [p_AlternativaId], [p_ObraId], [p_AcaoId], [p_ItemId], [p_TipoAcaoId], 
      IIF([p_EquipOriginalIdAux] = -1, NULL, [p_EquipOriginalIdAux]), 
      IIF([p_EquipEstadoPreIdAux] = -1, NULL, [p_EquipEstadoPreIdAux]),
      IIF([p_EquipEstadoPosIdAux] = -1, NULL, [p_EquipEstadoPosIdAux]) 
    );
    
    
    
