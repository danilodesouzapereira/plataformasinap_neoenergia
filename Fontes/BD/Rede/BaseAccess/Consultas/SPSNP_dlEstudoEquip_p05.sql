  DELETE FROM
    RRCarga
  WHERE
    intEstudoId = [p_EstudoId] AND
    intRRCargaId IN
    (
    SELECT
      Equipamento.intEquipId
    FROM
      Equipamento INNER JOIN RRCarga EqpAux ON
        Equipamento.intEstudoId = EqpAux.intEstudoId AND
        Equipamento.intEquipId = EqpAux.intRRCargaId
    WHERE
      Equipamento.intEstudoId = [p_EstudoId] AND
      (( [p_RedeInclusive] = 1) OR (Equipamento.bolAlteradoObra = 1)) AND
	    Equipamento.intTipoEquipId = 7
    );
