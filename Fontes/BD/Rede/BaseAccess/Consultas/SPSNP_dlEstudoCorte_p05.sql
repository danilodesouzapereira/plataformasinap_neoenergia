DELETE 
  RRCarga.*
FROM 
  RRCarga INNER JOIN TmpEquip ON
    RRCarga.intMRedeId = TmpEquip.intEquipId
WHERE 
  RRCarga.intEstudoId = [p_EstudoId];
