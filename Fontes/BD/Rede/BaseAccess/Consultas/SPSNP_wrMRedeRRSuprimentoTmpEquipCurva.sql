INSERT INTO TmpEquipCurva (intEquipId, intConjCurvaId)
SELECT
  Equipamento.intEquipId,
  IIF(ISNULL(intConjCurvaId), -1, intConjCurvaId)
FROM
 RRSuprimento RREquip INNER JOIN Equipamento ON 
    RREquip.intEstudoId = Equipamento.intEstudoId AND    
    RREquip.intRRSuprimentoId = Equipamento.intEquipId
WHERE
   RREquip.intEstudoId = [p_EstudoId] AND
   RREquip.intMRedeId = [p_MRedeId]
