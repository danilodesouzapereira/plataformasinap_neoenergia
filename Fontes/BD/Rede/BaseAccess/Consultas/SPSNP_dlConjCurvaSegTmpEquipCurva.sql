DELETE FROM ConjuntoCurvaSegmento WHERE intConjCurvaId IN (SELECT intConjCurvaId FROM TmpEquipCurva)
