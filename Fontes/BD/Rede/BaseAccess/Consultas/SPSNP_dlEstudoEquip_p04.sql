  DELETE FROM
    RRSuprimento
  WHERE
    intEstudoId = [p_EstudoId] AND
    intRRSuprimentoId IN
    (
    SELECT
      Equipamento.intEquipId
    FROM
      Equipamento INNER JOIN RRSuprimento EqpAux ON
        Equipamento.intEstudoId = EqpAux.intEstudoId AND
        Equipamento.intEquipId = EqpAux.intRRSuprimentoId
    WHERE
      Equipamento.intEstudoId = [p_EstudoId] AND
      (( [p_RedeInclusive] = 1) OR (Equipamento.bolAlteradoObra = 1)) AND
	    Equipamento.intTipoEquipId = 23
    );
