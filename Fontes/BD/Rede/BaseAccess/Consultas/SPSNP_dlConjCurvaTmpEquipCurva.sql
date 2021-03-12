DELETE FROM ConjuntoCurva WHERE intConjCurvaId IN (SELECT intConjCurvaId FROM TmpEquipCurva)
