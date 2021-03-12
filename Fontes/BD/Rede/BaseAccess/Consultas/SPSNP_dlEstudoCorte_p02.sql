DELETE 
  RRYRef.*
FROM 
  RRYRef INNER JOIN TmpEquip ON
    RRYRef.intMRedeId = TmpEquip.intEquipId
WHERE 
  RRYRef.intEstudoId = [p_EstudoId];